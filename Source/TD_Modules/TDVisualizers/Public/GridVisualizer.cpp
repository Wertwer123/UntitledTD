#include "GridVisualizer.h"

#include "TDGridComponent.h"

void FGridVisualizer::DrawGrid(const FGrid& GridToDraw, const UTDGridComponent* GridVisualizationComponent, FPrimitiveDrawInterface* PDI)
{
	if(GridToDraw.GetColumns() == 0 || GridToDraw.GetRows() == 0) return;
	
	FVector BottomLeft = FVector(-GridToDraw.GetGridHeight() / 2, -GridToDraw.GetGridWidth() / 2, 0);
	FVector BottomRight = FVector(GridToDraw.GetGridHeight() / 2, -GridToDraw.GetGridWidth() / 2, 0);
	FVector TopLeft = FVector(-GridToDraw.GetGridHeight() / 2, GridToDraw.GetGridWidth() / 2, 0);
	FVector XStep = FVector(GridToDraw.GetCellExtents() * 2, 0, 0);
	FVector YStep = FVector( 0, GridToDraw.GetCellExtents() * 2, 0);

	PDI->DrawPoint(GridToDraw.GetCells()[((GridToDraw.GetRows() * GridToDraw.GetColumns()) / 2)].Center, FColor::Red, 20, 0);
	
	for (int32 x = 0; x <= GridVisualizationComponent->GetGrids()->GetBaseGrid().GetRows(); ++x)
	{
		FVector DrawPointBottom = BottomLeft + (XStep * x);
		FVector DrawPointTop = TopLeft + (XStep * x);

		PDI->DrawLine(DrawPointBottom, DrawPointTop, FColor::Green, 0, GridVisualizationComponent->GetGridDrawWidth(), 0);
	}
	for(int32 y = 0; y <= GridVisualizationComponent->GetGrids()->GetBaseGrid().GetColumns(); ++y)
	{
		FVector DrawPointLeft = BottomLeft + (YStep * y);
		FVector DrawPointRight = BottomRight + (YStep * y);
		
		PDI->DrawLine(DrawPointLeft, DrawPointRight, FColor::Green, 0, GridVisualizationComponent->GetGridDrawWidth(), 0);
	}
}

void FGridVisualizer::DrawGridPosition(const FGrid& GridToDrawPositionIn, const UTDGridComponent* GridVisualizationComponent,  FPrimitiveDrawInterface* PDI)
{
	FGridCell Cell = GridToDrawPositionIn.GetCellAtPosition(GridVisualizationComponent->GetGridReferencePosition());
	FVector Position = Cell.Center;
	PDI->DrawPoint(Position, FColor::Blue, 40, 0);

	for (const auto& NeighbouringCell : GridToDrawPositionIn.GetNeighborsOfCell(Cell))
	{
		PDI->DrawPoint(NeighbouringCell.Center, FColor::Blue, 40, 0);
	}
}

void FGridVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                        FPrimitiveDrawInterface* PDI)
{
	FComponentVisualizer::DrawVisualization(Component, View, PDI);

	const UTDGridComponent* GridVisualizationComponent = Cast<UTDGridComponent>(Component);

	FGrid& BaseGrid = GridVisualizationComponent->GetGrids()->GetBaseGrid();
	
	if(BaseGrid.GetCells().IsEmpty()) return;
	
	DrawGrid(BaseGrid, GridVisualizationComponent, PDI);
	DrawGridPosition(BaseGrid, GridVisualizationComponent, PDI);
}
