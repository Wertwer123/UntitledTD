#pragma once

#include "CoreMinimal.h"
#include "AStarPathRequest.h"
#include "PathingAgent.h"
#include "Engine/DataAsset.h"
#include "TDGridCollection.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TDWorldGrids, Log, All)

UENUM()
enum class ELocation
{
	Top,
	TopRight,
	Right,
	BottomRight,
	Bottom,
	BottomLeft,
	Left,
	TopLeft
};

USTRUCT()
struct TDPATHFINDING_API FGridCell
{
	GENERATED_BODY()

	FGridCell(){}
	FGridCell(
		const FVector& InVector,
		const int32 InCellIndex,
		const int32 InRowIndex,
		const int32 InColumnIndex) :
	
		Center(InVector),
		CellIndex(InCellIndex),
		RowIndex(InRowIndex),
		ColumnIndex(InColumnIndex){}

	bool operator==(const FGridCell& Other) const
	{
		return RowIndex == Other.RowIndex && ColumnIndex == Other.ColumnIndex;
	}
	
	UPROPERTY()
	FVector Center = FVector::Zero();

	UPROPERTY()
	int32 CellIndex = 0;
	
	UPROPERTY()
	int32 RowIndex = 0;

	UPROPERTY()
	int32 ColumnIndex = 0;

	UPROPERTY()
	bool bIsBlocked = false;
};

USTRUCT()
struct TDPATHFINDING_API FGrid
{
	GENERATED_BODY()

	virtual ~FGrid(){}
	
private:
	UPROPERTY(EditAnywhere, Category= "Grid|Settings")
	int32 CellExtents = 100;
	
	UPROPERTY(EditAnywhere, Category= "Grid|Settings")
	int32 GridWidth = 100;

	UPROPERTY(EditAnywhere, Category= "Grid|Settings")
	int32 GridHeight = 100;

	UPROPERTY(EditDefaultsOnly, Category="Grid|Settings")
	FVector Center = FVector::Zero();
	
	UPROPERTY(VisibleAnywhere, Category= "Grid")
	TArray<FGridCell> Cells;

	UPROPERTY(VisibleAnywhere, Category= "Grid|Information")
	int32 Columns = 0;
	
	UPROPERTY(VisibleAnywhere, Category= "Grid|Information")
	int32 Rows = 0;
	
public:
	void EmptyGrid();
	void AddGridCell(const FGridCell& CellToAdd);
	void SetCenter(const FVector& NewCenter);
	void SetRows();
	void SetColumns();
	void SetCellExtents(const int32 NewCellExtents);
	void SetGridHeightAndWidth(const int32 NewGridWidth, const int32 NewGridHeight);
	virtual void GenerateGrid();

	int32 GetGridWidth () const;
	int32 GetGridHeight() const;
	int32 GetCellExtents() const {return CellExtents;}
	int32 GetColumns() const {return Columns;}
	int32 GetRows() const {return Rows;}
	static int32 GetCellIndex(const FGridCell& GridCell);
	FVector GetGridCenter() const {return Center;}
	FVector GetLowerLeftEdgeOfGrid() const;
	TArray<FGridCell> GetNeighborsOfCell(const FGridCell& GridCellToGetNeighborsFrom) const;
	//An slower version of get neighbors of cell but also returns their locations with them
	TMap<ELocation, FGridCell> GetNeighborsOfCellMapped(const FGridCell& GridCellToGetNeighborsFrom) const;
	const FGridCell& GetCellAtPosition(const FVector& PositionToGetCellAt) const;
	const TArray<FGridCell>& GetCells() const {return Cells;} 
};

USTRUCT()
struct TDPATHFINDING_API FCachedPath
{
	GENERATED_BODY()
	TArray<int32> CachedPath;
};
//Describes the literal connection between two clusters by holding the index from where we start our journey and to which cell we are travelling by this path
//on a high level
USTRUCT()
struct FClusterConnection
{
	GENERATED_BODY()

	FClusterConnection(){};
	FClusterConnection(
		const int32 CellIndexEntranceConnection,
		const int32 CellIndexExitConnection,
		const int32 HighLevelFromCellIndex,
		const int32 HighLevelToCellIndex) :

		CellIndexEntranceConnection(CellIndexEntranceConnection),
		CellIndexExitConnection(CellIndexExitConnection),
		HighLevelFromCellIndex(HighLevelFromCellIndex),
		HighLevelToCellIndex(HighLevelToCellIndex){}
	
	UPROPERTY()
	int32 CellIndexEntranceConnection = 0;

	UPROPERTY()
	int32 CellIndexExitConnection = 0;

	//This represents the cell this connection resides in
	UPROPERTY()
	int32 HighLevelFromCellIndex = 0;

	UPROPERTY()
	int32 HighLevelToCellIndex = 0;

	//Key is the high level cell that we want to reach
	UPROPERTY()
	TMap<int32, FCachedPath> CachedPaths;
};

//A cluster edge describes the whole edge of a cluster in form of the cell indices that are on a edge
USTRUCT()
struct FClusterEdge
{
	GENERATED_BODY()

	FClusterEdge(){};
	FClusterEdge(const int32 MiddleIndexOfEdge, const TArray<int32>& EdgeIndices) :
	MiddleIndexOfEdge(MiddleIndexOfEdge),
	EdgeIndices(EdgeIndices){};
	
	UPROPERTY()
	int32 MiddleIndexOfEdge = 0;
	
	UPROPERTY()
	TArray<int32> EdgeIndices;
};
USTRUCT()
struct TDPATHFINDING_API FCluster
{
	GENERATED_BODY()

	FCluster(){}	
	FCluster(const int32 HighLevelGridHeightAndWidth, const int32 CellExtents, const FVector& Center);
	
	UPROPERTY()
	FGrid SubGrid;
	
	UPROPERTY()
	TArray<FClusterConnection> Connections{};

	bool TryGetFreeCellOnEdge(const ELocation Location, FGridCell& OutCell) const;
	//Gives you the corresponding cell index of a entrance cell
	int32 GetSubgridCellEntranceIndexByLocation(const ELocation Location) const;
	int32 GetSubgridCellEntranceIndexRelativeToLocation(const ELocation Location) const;

	FORCEINLINE FClusterConnection GetClusterConnection(int32 HighLevelToCellIndex) const
	{
		for (const FClusterConnection& ClusterConnection : Connections)
		{
			if(ClusterConnection.HighLevelToCellIndex == HighLevelToCellIndex)
			{
				return ClusterConnection;
			}
		}
		
		return {};
	};
	//Gives you the exit connection to the cell you want to get to
	FORCEINLINE bool  TryGetClusterConnection(int32 HighLevelToCellIndex, FClusterConnection& OutFoundConnection) const
	{
		for (const FClusterConnection& ClusterConnection : Connections)
		{
			if(ClusterConnection.HighLevelToCellIndex == HighLevelToCellIndex)
			{
				OutFoundConnection = ClusterConnection;
				return true;
			}
		}
		
		return false;
	};
	FORCEINLINE void SetClusterConnectionCachedPath(const FAStarPathfindingResult& PathfindingResult)
	{
		for (FClusterConnection& ClusterConnection : Connections)
		{
			if(ClusterConnection.CellIndexEntranceConnection == PathfindingResult.FromCellIndex)
			{
				ClusterConnection.CachedPaths.Add( PathfindingResult.HighLevelGridCellIndex,{PathfindingResult.CellsToPass});
				return;
			}
		}
	};

private:
	TMap<ELocation,FClusterEdge> Edges;
	
	void FormClusterEdges();
	static FClusterEdge GenerateEdge(const int32 UpperBounds, const int32 StartIndex, const int32 Increment);
};

UCLASS()
class TDPATHFINDING_API UTDGridCollection : public UDataAsset, public IPathingAgent
{
	GENERATED_BODY()
public:
	//1
	void GenerateGrids(UObject* Outer);
	virtual void RecievePath(FAStarPathfindingResult& PathfindingResult) override;
	
	int32 GetNumClusterGridCells() const {return ClusterGridCells;}
	FGrid& GetMutableBaseGrid() {return BaseGrid;}
	TArray<FCluster>& GetMutableClusters() {return Clusters;}

private:
	UPROPERTY(EditDefaultsOnly, Category= "Grid|Clusters")
	int32 ClusterGridCells = 20;

	UPROPERTY(VisibleDefaultsOnly, Category= "Grid|Clusters")
	int32 TotalClusterConnections = 0;

	UPROPERTY(VisibleDefaultsOnly, Category= "Grid|Clusters")
	int32 RecievedClusterConnections = 0;
	
	UPROPERTY(EditDefaultsOnly, Category= "Grid")
	FGrid BaseGrid;

	//These cluster represent the abstract path graph
	UPROPERTY(VisibleDefaultsOnly, Category= "Grid|PathfindingGrid")
	TArray<FCluster> Clusters;

	FCriticalSection CounterIncrease;
	TQueue<FAStarPathfindingResult, EQueueMode::Spsc> CollectedResults;
	
	//2
	void GenerateClusters(UObject* Outer);

	//3
	void BuildClusterConnections(const TMap<ELocation, FGridCell>& NeighboringHighLevelCells, const int32 HighLevelGridCellIndex, FCluster& OutCluster, UObject* Outer);

	//4
	void BuildInterConnections(const TArray<FCluster>& CreatedClusters, UObject* Outer);
};
