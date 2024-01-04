#include "GridVisualizer.h"

#include "TDGridComponent.h"

void FGridVisualizer::DrawGrid(const FGrid& GridToDraw, const UTDGridComponent* GridVisualizationComponent, const FColor& GridColor, FPrimitiveDrawInterface* PDI)
{
	if(GridToDraw.GetColumns() == 0 || GridToDraw.GetRows() == 0) return;
	if(GridToDraw.GetCells().IsEmpty()) return;
	
	const FVector GridCenter = GridToDraw.GetGridCenter();
	const FVector BottomLeft = GridCenter + FVector(-GridToDraw.GetGridHeight() / 2, -GridToDraw.GetGridWidth() / 2, 0);
	const FVector BottomRight = GridCenter + FVector(GridToDraw.GetGridHeight() / 2, -GridToDraw.GetGridWidth() / 2, 0);
	const FVector TopLeft = GridCenter + FVector(-GridToDraw.GetGridHeight() / 2, GridToDraw.GetGridWidth() / 2, 0);
	const FVector XStep = FVector(GridToDraw.GetCellExtents() * 2, 0, 0);
	const FVector YStep =  FVector( 0, GridToDraw.GetCellExtents() * 2, 0);
	
	for (int32 x = 0; x <= GridToDraw.GetRows(); ++x)
	{
		FVector DrawPointBottom = BottomLeft + (XStep * x);
		FVector DrawPointTop = TopLeft + (XStep * x);
		
		PDI->DrawLine(DrawPointBottom, DrawPointTop, GridColor, 0, GridVisualizationComponent->GetGridDrawWidth(), 0);
	}
	for(int32 y = 0; y <= GridToDraw.GetColumns(); ++y)
	{
		FVector DrawPointLeft = BottomLeft + (YStep * y);
		FVector DrawPointRight = BottomRight + (YStep * y);
		
		PDI->DrawLine(DrawPointLeft, DrawPointRight, GridColor, 0, GridVisualizationComponent->GetGridDrawWidth(), 0);
	}

	PDI->DrawPoint(GridCenter, GridColor, GridVisualizationComponent->GetGridDrawWidth(), 0);
}

void FGridVisualizer::DrawGridPosition(const FGrid& GridToDrawPositionIn, const UTDGridComponent* GridVisualizationComponent,  FPrimitiveDrawInterface* PDI)
{
	FGridCell Cell = GridToDrawPositionIn.GetCellAtPosition(GridVisualizationComponent->GetGridReferencePosition());
	FVector Position = Cell.Center;
	FVector OriginCell = GridToDrawPositionIn.GetCells()[0].Center;
	PDI->DrawPoint(Position, FColor::Blue, 10, 0);
	PDI->DrawLine(OriginCell, Position, FColor::Blue, 40, 0);
}

void FGridVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                        FPrimitiveDrawInterface* PDI)
{
	FComponentVisualizer::DrawVisualization(Component, View, PDI);

	const UTDGridComponent* GridVisualizationComponent = Cast<UTDGridComponent>(Component);

	FGrid& BaseGrid = GridVisualizationComponent->GetGrids()->GetMutableBaseGrid();
	const TArray<FCluster>& Clusters = GridVisualizationComponent->GetGrids()->GetMutableClusters();
	
	if(BaseGrid.GetCells().IsEmpty()) return;

	if(GridVisualizationComponent->DrawGrids())
	{
		DrawGrid(BaseGrid, GridVisualizationComponent, FColor::Green, PDI);
	}
	
	for (const FCluster& Cluster : GridVisualizationComponent->GetGrids()->GetMutableClusters())
	{
		if(Cluster.Connections.IsEmpty()) continue;
		if(GridVisualizationComponent->DrawSubGrids())
		{
			DrawGrid(Cluster.SubGrid, GridVisualizationComponent, FColor::Blue, PDI);
		}
		
		// DrawClusterExits(Cluster, GridVisualizationComponent, PDI);
	}
	
	DrawGridPosition(BaseGrid, GridVisualizationComponent, PDI);
	DrawGridPosition(Clusters[BaseGrid.GetCellAtPosition(GridVisualizationComponent->GetGridReferencePosition()).CellIndex].SubGrid, GridVisualizationComponent, PDI);
}

void FGridVisualizer::DrawClusterExits(const FCluster& InCLusterToDrawConnectionsFor,
	const UTDGridComponent* GridComponent, FPrimitiveDrawInterface* PDI)
{
	for (const FClusterConnection& ClusterConnection : InCLusterToDrawConnectionsFor.Connections)
	{
		const FVector& DrawPosition = InCLusterToDrawConnectionsFor.SubGrid.GetCells()[ClusterConnection.CellIndexEntranceConnection].Center;
		PDI->DrawPoint(DrawPosition, FColor::Cyan, 15, 0);
	}
}
