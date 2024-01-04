#include "TDGridCollection.h"

#include "PathFindingSubsystem.h"

DEFINE_LOG_CATEGORY(TDWorldGrids)

void FGrid::GenerateGrid()
{
	EmptyGrid();
	
	FVector PointInGrid = GetLowerLeftEdgeOfGrid();

	SetRows();
	SetColumns();
	
	const int32 GridSize = Rows * Columns;
	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;
	
	if(Rows == 0 || Columns == 0)
	{
		UE_LOG(TDWorldGrids, Error, TEXT("The Grid was to small for the cells"))
		return;
	};
	
	for (int32 i = 0, j = 0; i < GridSize; ++i, ++j)
	{
		FVector GridPositionIncrement;
		
		if(j == Rows)
		{
			PointInGrid.Y = Center.Y -GridWidth / 2;
			PointInGrid.X += CellExtents * 2;
			CurrentColumn = 0;
			j = 0;
			CurrentRow++;
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

void FGrid::SetCenter(const FVector& NewCenter)
{
	Center = NewCenter;
}

void FGrid::SetRows()
{
	Rows = GridWidth / (CellExtents * 2);
}

void FGrid::SetColumns()
{
	Columns = GridHeight / (CellExtents * 2);
}

void FGrid::SetCellExtents(const int32 NewCellExtents)
{
	CellExtents = NewCellExtents;
}

void FGrid::SetGridHeightAndWidth(const int32 NewGridWidth, const int32 NewGridHeight)
{
	GridWidth = NewGridWidth;
	GridHeight = NewGridHeight;
}

int32 FGrid::GetCellIndex(const FGridCell& GridCell)
{
	return GridCell.CellIndex;
}

FVector FGrid::GetLowerLeftEdgeOfGrid() const
{
	return Center + FVector(
		-GridWidth / 2,
		-GridHeight / 2,
		0);
}

TArray<FGridCell> FGrid::GetNeighborsOfCell(const FGridCell& GridCellToGetNeighborsFrom) const
{
	TArray<FGridCell> NeighbouringCells;
	
	const int32 CellRowIndex = GridCellToGetNeighborsFrom.RowIndex;
	const int32 CellColumnIndex = GridCellToGetNeighborsFrom.ColumnIndex;

	//(Row Index) * OverallColumnLenght  + (CollumnIndex) == CurrentCell
	const int32 TopCellIndex = (CellRowIndex + 1) * GetColumns() + CellColumnIndex;
	int32 TopRightCellIndex = (CellRowIndex + 1) * GetColumns() + CellColumnIndex + 1;
	int32 RightCellIndex = CellRowIndex  * GetColumns() + CellColumnIndex + 1;
	int32 BottomRightCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex + 1;
	
	const int32 BottomCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex;
	int32 BottomLeftCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex - 1;
	int32 LeftCellIndex = CellRowIndex * GetColumns() + CellColumnIndex - 1;
	int32 TopLeftCellIndex = (CellRowIndex + 1) * GetColumns() + CellColumnIndex - 1;
	
	if(GridCellToGetNeighborsFrom.ColumnIndex == Columns - 1)
	{
		TopRightCellIndex = -1;
		RightCellIndex = -1;
		BottomRightCellIndex = -1;
	}
	else if(GridCellToGetNeighborsFrom.ColumnIndex == 0)
	{
		LeftCellIndex = -1;
		BottomLeftCellIndex = -1;
		TopLeftCellIndex = -1;	
	}
	
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

TMap<ELocation, FGridCell> FGrid::GetNeighborsOfCellMapped(const FGridCell& GridCellToGetNeighborsFrom) const
{
	TMap<ELocation, FGridCell> NeighbouringCellsMapped;
	
	const int32 CellRowIndex = GridCellToGetNeighborsFrom.RowIndex;
	const int32 CellColumnIndex = GridCellToGetNeighborsFrom.ColumnIndex;

	//(Row Index) * OverallColumnLenght  + (CollumnIndex) == CurrentCell
	const int32 TopCellIndex = (CellRowIndex + 1) * GetColumns() + CellColumnIndex;
	int32 TopRightCellIndex = (CellRowIndex + 1) * GetColumns() + CellColumnIndex + 1;
	int32 RightCellIndex = CellRowIndex  * GetColumns() + CellColumnIndex + 1;
	int32 BottomRightCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex + 1;
	const int32 BottomCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex;
	int32 BottomLeftCellIndex = (CellRowIndex - 1) * GetColumns() + CellColumnIndex - 1;
	int32 LeftCellIndex = CellRowIndex * GetColumns() + CellColumnIndex - 1;
	int32 TopLeftCellIndex = (CellRowIndex + 1) * GetColumns() + CellColumnIndex - 1;

	if(GridCellToGetNeighborsFrom.ColumnIndex == Columns - 1)
	{
		TopRightCellIndex = -1;
		RightCellIndex = -1;
		BottomRightCellIndex = -1;
	}
	else if(GridCellToGetNeighborsFrom.ColumnIndex == 0)
	{
		LeftCellIndex = -1;
		BottomLeftCellIndex = -1;
		TopLeftCellIndex = -1;	
	}
	
	TMap<ELocation,int32> CellIndices
	{
		{ELocation::Top, TopCellIndex},
		{ELocation::TopRight, TopRightCellIndex},
		{ELocation::Right, RightCellIndex},
		{ELocation::BottomRight, BottomRightCellIndex},
		{ELocation::Bottom, BottomCellIndex},
		{ELocation::BottomLeft, BottomLeftCellIndex},
		{ELocation::Left, LeftCellIndex},
		{ELocation::TopLeft, TopLeftCellIndex},
	};

	for (auto&[CellLocation, CellIndex] : CellIndices)
	{
		if(Cells.IsValidIndex(CellIndex))
		{
			NeighbouringCellsMapped.Add(CellLocation, Cells[CellIndex]);
		}
	}
	
	return NeighbouringCellsMapped;
}

const FGridCell& FGrid::GetCellAtPosition(const FVector& PositionToGetCellAt) const
{
	const int32 GridSize = GetColumns() * GetRows();
	constexpr int32 OriginCellIndex = 0;
	
	const FGridCell& OriginCell = Cells[OriginCellIndex];
	const int32 CellPositionX = FMath::RoundToInt32(static_cast<float>((PositionToGetCellAt.Y - OriginCell.Center.Y)  / (CellExtents * 2)));
	const int32 CellPositionY = FMath::RoundToInt(static_cast<float>((PositionToGetCellAt.X - OriginCell.Center.X)  / (CellExtents * 2)));
	
	int32 CellIndex = CellPositionY * Rows + CellPositionX;
	
	CellIndex = FMath::Clamp(CellIndex, 0, GridSize - 1);
	return Cells[CellIndex];
}

FCluster::FCluster(const int32 HighLevelGridHeightAndWidth, const int32 CellExtents, const FVector& Center)
{
	const int32 GridWidth = HighLevelGridHeightAndWidth;
	const int32 GridHeight = HighLevelGridHeightAndWidth;

	//Generate the subgrid
	SubGrid.SetCenter(Center);
	SubGrid.SetGridHeightAndWidth(GridWidth, GridHeight);
	SubGrid.SetCellExtents(CellExtents);
	SubGrid.GenerateGrid();
	//Generate the edges of the subgrid
	FormClusterEdges();
}

bool FCluster::TryGetFreeCellOnEdge(const ELocation Location, FGridCell&  OutCell) const
{
	const FClusterEdge& Edge = Edges[Location];
	
	for (const int32 EdgeIndex : Edge.EdgeIndices)
	{
		if(!SubGrid.GetCells()[EdgeIndex].bIsBlocked)
		{
			OutCell = SubGrid.GetCells()[EdgeIndex];
			return true;
		}
	}

	return false;
}

int32 FCluster::GetSubgridCellEntranceIndexByLocation(const ELocation Location) const
{
	const int32 GridRows = SubGrid.GetRows();
	const int32 GridColumns = SubGrid.GetColumns();
	
	switch (Location)
	{
	case ELocation::Top:
	{
		const int32 RowCells = (GridRows - 1) * GridColumns;
		const int32 ColumnCells = GridColumns * 0.5;
		const int32 CellIndexTop = RowCells + ColumnCells;
		return SubGrid.GetCells()[CellIndexTop - 1].CellIndex;
	}
	case ELocation::TopRight:
	{
		const int32 RowCells = (GridRows * GridColumns) - 1;
		const int32 CellIndexTopRight = RowCells;
		return SubGrid.GetCells()[CellIndexTopRight].CellIndex;
	}
	case ELocation::Right:
	{
		const int32 RowCells = (GridRows - 1) * 0.5  * GridColumns;
		const int32 CellIndexRight = RowCells - 1;
		return SubGrid.GetCells()[CellIndexRight].CellIndex;
	}
	case ELocation::BottomRight:
	{
		const int32 RowCells = GridRows;
		const int32 CellIndexBottomRight = RowCells - 1;
		return SubGrid.GetCells()[CellIndexBottomRight].CellIndex;
	}
	case ELocation::Bottom:
	{
		const int32 RowCells = (GridRows * 0.5) - 1;
		const int32 CellIndexBottom = RowCells;
		return SubGrid.GetCells()[CellIndexBottom].CellIndex;
	}
	case ELocation::BottomLeft:
	{
		constexpr int32 RowCells = 0;
		constexpr int32 CellIndexBottomLeft = RowCells;
		return SubGrid.GetCells()[CellIndexBottomLeft].CellIndex;
	}
	case ELocation::Left:
	{
		const int32 RowCells = (GridRows * 0.5 - 1) * GridColumns;
		const int32 CellIndexLeft = RowCells;
		return SubGrid.GetCells()[CellIndexLeft].CellIndex;
	}
	case ELocation::TopLeft:
	{
		const int32 RowCells = (GridRows - 1) * GridColumns;
		const int32 CellIndexTopLeft = RowCells;
		return SubGrid.GetCells()[CellIndexTopLeft].CellIndex;
	}
	default: return 0;
	}
}

int32 FCluster::GetSubgridCellEntranceIndexRelativeToLocation(const ELocation Location) const
{
	switch (Location)
	{
	case ELocation::Top:
		return GetSubgridCellEntranceIndexByLocation(ELocation::Bottom);
	case ELocation::TopRight:
		return GetSubgridCellEntranceIndexByLocation(ELocation::BottomLeft);
	case ELocation::Right:
		return GetSubgridCellEntranceIndexByLocation(ELocation::Left);
	case ELocation::BottomRight:
		return GetSubgridCellEntranceIndexByLocation(ELocation::TopLeft);
	case ELocation::Bottom:
		return GetSubgridCellEntranceIndexByLocation(ELocation::Top);
	case ELocation::BottomLeft:
		return GetSubgridCellEntranceIndexByLocation(ELocation::TopRight);
	case ELocation::Left:
		return GetSubgridCellEntranceIndexByLocation(ELocation::Right);
	case ELocation::TopLeft:
		return GetSubgridCellEntranceIndexByLocation(ELocation::BottomRight);
	default: return 0;
	}
}

void FCluster::FormClusterEdges()
{
	const int32 GridRows = SubGrid.GetRows();
	const int32 GridColumns = SubGrid.GetColumns();
	
	//Top Edge
	constexpr int32 Increment = 1;
	const int32 Bounds = GridRows * GridColumns;
	const FClusterEdge TopEdge = GenerateEdge(Bounds, GetSubgridCellEntranceIndexByLocation(ELocation::Top), Increment);
	
	//Right Edge
	const int32 RightEdgeIncrement = GridColumns;
	const int32 RightEdgeBounds = GridRows * GridColumns - 1;
	const FClusterEdge RightEdge = GenerateEdge(RightEdgeBounds, GetSubgridCellEntranceIndexByLocation(ELocation::Right), RightEdgeIncrement);

	//Bottom Edge
	constexpr int32 BottomEdgeIncrement = 1;
	const int32 BottomEdgeBounds = GridColumns;
	const FClusterEdge BottomEdge = GenerateEdge(BottomEdgeBounds, GetSubgridCellEntranceIndexByLocation(ELocation::Bottom), BottomEdgeIncrement);

	//Left Edge
	const int32 LeftEdgeIncrement = GridColumns;
	const int32 LeftEdgeBounds = GridColumns * (GridRows - GridColumns - 1);
	const FClusterEdge LeftEdge = GenerateEdge(LeftEdgeBounds, GetSubgridCellEntranceIndexByLocation(ELocation::Left), LeftEdgeIncrement);

	//Corners
	const int32 TopRightEdgeIndex = GetSubgridCellEntranceIndexByLocation(ELocation::TopRight);
	const int32 BottomRightEdgeIndex = GetSubgridCellEntranceIndexByLocation(ELocation::BottomRight);
	const int32 BottomLeftEdgeIndex = GetSubgridCellEntranceIndexByLocation(ELocation::BottomLeft);
	const int32 TopLeftEdgeIndex = GetSubgridCellEntranceIndexByLocation(ELocation::TopLeft);
	
	const FClusterEdge TopRightEdge(TopRightEdgeIndex, {TopRightEdgeIndex});
	const FClusterEdge BottomRightEdge(BottomLeftEdgeIndex, {BottomRightEdgeIndex});
	const FClusterEdge BottomLeftEdge(BottomLeftEdgeIndex, {BottomLeftEdgeIndex});
	const FClusterEdge TopLeftEdge(TopLeftEdgeIndex, {TopLeftEdgeIndex});
	
	//Add the edges of the subgrid
	Edges.Add(ELocation::Top, TopEdge);
	Edges.Add(ELocation::TopRight, TopRightEdge);
	Edges.Add(ELocation::Right, RightEdge);
	Edges.Add(ELocation::BottomRight, BottomRightEdge);
	Edges.Add(ELocation::Bottom, BottomEdge);
	Edges.Add(ELocation::BottomLeft, BottomLeftEdge);
	Edges.Add(ELocation::Left, LeftEdge);
	Edges.Add(ELocation::TopLeft, TopLeftEdge);
}

FClusterEdge FCluster::GenerateEdge(const int32 UpperBounds, const int32 StartIndex, const int32 Increment)
{
	FClusterEdge Edge;
	Edge.EdgeIndices.Add(StartIndex);
	
	for (int32 i = StartIndex + Increment, j = StartIndex - Increment; i < UpperBounds; i+= Increment, j-= Increment)
	{
		Edge.EdgeIndices.Emplace(i);
		Edge.EdgeIndices.Emplace(j);
	}

	return Edge;
}

void UTDGridCollection::GenerateGrids(UObject* Outer)
{
	BaseGrid.GenerateGrid();

	GenerateClusters(Outer);
}

void UTDGridCollection::RecievePath(FAStarPathfindingResult& PathfindingResult)
{
	CounterIncrease.Lock();
	
	RecievedClusterConnections += CollectedResults.Enqueue(PathfindingResult);

	CounterIncrease.Unlock();
	
	if(RecievedClusterConnections == TotalClusterConnections)
	{
		while (!CollectedResults.IsEmpty())
		{
			const FAStarPathfindingResult PathFindingResults;
			CollectedResults.Dequeue(PathfindingResult);
			Clusters[PathfindingResult.HighLevelGridCellIndex].SetClusterConnectionCachedPath(PathfindingResult);
		}
	}
}

void UTDGridCollection::GenerateClusters(UObject* Outer)
{
	Clusters.Empty();
	CollectedResults.Empty();
	TotalClusterConnections = 0;
	RecievedClusterConnections = 0;
	
	for (const FGridCell& GridCell : BaseGrid.GetCells())
	{
		const int32 ClusterGridWidthAndHeight = BaseGrid.GetCellExtents() * 2;
		const int32 ClusterGridCellExtents = (ClusterGridWidthAndHeight / ClusterGridCells) / 2;
		
		FCluster CellCluster(ClusterGridWidthAndHeight, ClusterGridCellExtents, GridCell.Center);
		BuildClusterConnections(BaseGrid.GetNeighborsOfCellMapped(GridCell), GridCell.CellIndex, CellCluster, Outer);
		Clusters.Add(CellCluster);
		//Foreach cluster build the cluster Connections to the other cells
		
	}

	BuildInterConnections(Clusters, Outer);
}

//This can also be used to regenerate the connections of a single cluster
void UTDGridCollection::BuildClusterConnections(const TMap<ELocation, FGridCell>& NeighboringHighLevelCells, const int32 HighLevelGridCellIndex, FCluster& OutCluster, UObject* Outer)
{
	for (const auto&[CellLocation, NeighborHighLevelCell] : NeighboringHighLevelCells)
	{
		FGridCell OutCell;
		const bool FoundEdge = OutCluster.TryGetFreeCellOnEdge(CellLocation, OutCell);

		if(!FoundEdge) continue;
		
		const int32 CellConnectionIndex = OutCell.CellIndex;
		//Because we dont have a neighboring cluster at this point we just take the index by location in this grid but because the grid is uniform it doesnt matter
		FClusterConnection Connection(CellConnectionIndex,OutCluster.GetSubgridCellEntranceIndexRelativeToLocation(CellLocation), HighLevelGridCellIndex, NeighborHighLevelCell.CellIndex);
		OutCluster.Connections.Add(Connection);
	}
	
	TotalClusterConnections += OutCluster.Connections.Num() * (OutCluster.Connections.Num() - 1);
}

void UTDGridCollection::BuildInterConnections(const TArray<FCluster>& CreatedClusters, UObject* Outer)
{
	//i resembles the High level Grid cell index as we have created the clusters in the same order
	for (int32 i = 0; i < CreatedClusters.Num(); ++i)
	{
		const FCluster& Cluster = CreatedClusters[i];
		
		//Form interconnections
		for (const FClusterConnection& Connection : Cluster.Connections)
		{
			UPathFindingSubsystem* PathfindingSubsystem = Outer->GetWorld()->GetSubsystem<UPathFindingSubsystem>();

			for (const FClusterConnection& ClusterConnection : Cluster.Connections)
			{
				//if we are not ourselves form a connection via astar 
				if(ClusterConnection.CellIndexEntranceConnection != Connection.CellIndexEntranceConnection)
				{
					//Pass in the clusters to connection as the highlevel cell so that we can easily access them when we construct the high level path
					//with hpa
					FAStarPathRequest PathRequest( ClusterConnection.HighLevelToCellIndex, Connection.CellIndexEntranceConnection, ClusterConnection.CellIndexEntranceConnection);
					PathfindingSubsystem->RequestAStarPath(PathRequest, MakeShared<FGrid>(Cluster.SubGrid), this);
				}
			}
		}
	}
}

