#include "TDGridCollection.h"

DEFINE_LOG_CATEGORY(TDWorldGrids)

void FGrid::GenerateGrid()
{
	EmptyGrid();
	
	FVector PointInGrid(
		-GridWidth / 2,
		-GridHeight / 2,
		0);

	int32 CellsPerRow = GridWidth / (CellExtents * 2);
	int32 CellsPerColumn = GridHeight / (CellExtents * 2);
	int32 GridSize = CellsPerRow * CellsPerColumn;
	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;
	
	Rows = CellsPerRow;
	Columns = CellsPerColumn;
	
	if(CellsPerRow == 0 || CellsPerColumn == 0)
	{
		UE_LOG(TDWorldGrids, Error, TEXT("The Grid was to small for the cells"))
		return;
	};
	
	for (int32 i = 0; i < GridSize; ++i)
	{
		int32 WidthReached = i % CellsPerRow;
		FVector GridPositionIncrement;
		
		if(WidthReached == 0)
		{
			GridPositionIncrement = FVector(CellExtents * 2, CellExtents * 2, 0);
			PointInGrid.Y = -GridWidth / 2;
			CurrentRow++;
			CurrentColumn = 0;
		}
		else
		{
			GridPositionIncrement = FVector( 0, CellExtents * 2, 0);
		}
		
		FGridCell NewGridCell
		(
			PointInGrid + FVector(CellExtents, CellExtents, 0),
			i,
			CurrentRow,
			CurrentColumn
		);
		
		PointInGrid += GridPositionIncrement;

		AddGridCell(NewGridCell);
		CurrentColumn++;
	}
}

int32 FGrid::GetGridWidth() const
{
	return GridWidth;
}

int32 FGrid::GetGridHeight() const
{
	return GridHeight;
}

void FGrid::EmptyGrid()
{
	Cells.Empty();
}

void FGrid::AddGridCell(const FGridCell& CellToAdd)
{
	Cells.Emplace(CellToAdd);
}

void FGrid::SetRows(const int32 InRows)
{
	Rows = InRows;
}

void FGrid::SetColumns(const int32 InColumns)
{
	Columns = InColumns;
}

TArray<FGridCell> FGrid::GetNeighborsOfCell(const FGridCell& GridCellToGetNeighborsFrom) const
{
	TArray<FGridCell> NeighbouringCells;
	
	const int32 CellRowIndex = GridCellToGetNeighborsFrom.RowIndex;
	const int32 CellColumnIndex = GridCellToGetNeighborsFrom.ColumnIndex;

	//(Row Index) * OverallColumnLenght  + (CollumnIndex) == CurrentCell
	const int32 TopCellIndex = (CellRowIndex ) * GetColumns() + CellColumnIndex;
	const int32 TopRightCellIndex = (CellRowIndex) * GetColumns() + CellColumnIndex + 1;
	const int32 RightCellIndex = CellRowIndex  * GetColumns() + CellColumnIndex + 1;
	const int32 BottomRightCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex + 1;
	const int32 BottomCellIndex = (CellRowIndex -2) * GetColumns() + CellColumnIndex;
	const int32 BottomLeftCellIndex = (CellRowIndex -2) * GetColumns() + CellColumnIndex - 1;
	const int32 LeftCellIndex = CellRowIndex * GetColumns() + CellColumnIndex - 1;
	const int32 TopLeftCellIndex = (CellRowIndex) * GetColumns() + CellColumnIndex - 1;

	TArray<int32> CellIndices
	{
		TopCellIndex,
		TopRightCellIndex,
		RightCellIndex,
		BottomRightCellIndex,
		BottomCellIndex,
		BottomLeftCellIndex,
		LeftCellIndex,
		TopLeftCellIndex
	};

	for (const int32 CellIndex : CellIndices)
	{
		if(Cells.IsValidIndex(CellIndex))
		{
			NeighbouringCells.Add(Cells[CellIndex]);
		}
	}
	
	return NeighbouringCells;
}

const FGridCell& FGrid::GetCellAtPosition(const FVector& PositionToGetCellAt) const
{
	const int32 GridSize = GetColumns() * GetRows();
	const int32 OriginCellIndex = 0;
	
	const FGridCell& OriginCell = Cells[OriginCellIndex];
	const int32 CellPositionX = FMath::RoundToInt32(static_cast<float>((PositionToGetCellAt.Y - OriginCell.Center.Y)  / (CellExtents * 2)));
	const int32 CellPositionY = FMath::RoundToInt(static_cast<float>((PositionToGetCellAt.X - OriginCell.Center.X)  / (CellExtents * 2)));

	//IDK why i have to subtract one from the cell posy but it do be work appearently lul
	int32 CellIndex =  (CellPositionY-1) * Rows + CellPositionX;
	
	CellIndex = FMath::Clamp(CellIndex, 0, GridSize - 1);
	return Cells[CellIndex];
}

void UTDGridCollection::GenerateGrids()
{
	BaseGrid.GenerateGrid();
}
