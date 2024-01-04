#include "PathFindingSubsystem.h"
#include "AStarPathRequest.h"
#include "Tasks/Task.h"

class ATD_GameMode;
DECLARE_STATS_GROUP(TEXT("Pathfinding"), STATGROUP_Pathfinding, STATCAT_Pathfinding)

DECLARE_CYCLE_STAT(TEXT("AStarAverageTime"), STAT_TDAI_Pathfinding, STATGROUP_Pathfinding)

void UPathFindingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	BaseGrid = MakeUnique<FGrid>(GridCollection->GetMutableBaseGrid());
}

FAStarPathRequest UPathFindingSubsystem::CreateRequest(const FVector& StartLocation, const FVector& EndLocation)
{
	const int32 StartCellHighLevel = BaseGrid->GetCellAtPosition(StartLocation).CellIndex;
	const int32 TargetCellHighLevel = BaseGrid->GetCellAtPosition(EndLocation).CellIndex;

	const FAStarPathRequest Request(StartCellHighLevel, StartCellHighLevel, TargetCellHighLevel, StartLocation, EndLocation);
	
	return Request;
}

void UPathFindingSubsystem::RequestAStarPath(const FAStarPathRequest& PathRequest, const TSharedPtr<FGrid>& GridToRunAStarOn, TWeakObjectPtr<UObject> AgentWhoRequestedPath)
{
	SCOPE_CYCLE_COUNTER(STAT_TDAI_Pathfinding)
	
	UE::Tasks::FTask AStarRequestTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, GridToRunAStarOn, PathRequest, AgentWhoRequestedPath]
	{
		IPathingAgent* PathingAgent = Cast<IPathingAgent>(AgentWhoRequestedPath);
		
		if(!PathingAgent) return;
		
		FAStarPathfindingResult Result = RunAStar(GridToRunAStarOn.Get(), PathRequest);
		
		if(AgentWhoRequestedPath.IsValid())
		{
			PathingAgent->RecievePath(Result);
		}
	});
}

void UPathFindingSubsystem::RequestHPAStarPath(const FAStarPathRequest& PathRequest,TWeakObjectPtr<UObject> AgentWhoRequestedPath)
{
	//Run HighLevel astar to find cells that we need to move through
	
	TWeakObjectPtr<UTDGridCollection> InGridCollection = GridCollection;
	
	UE::Tasks::FTask HPAStarRequest = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, PathRequest, InGridCollection, AgentWhoRequestedPath]
	{
		SCOPE_CYCLE_COUNTER(STAT_TDAI_Pathfinding)
		const FAStarPathfindingResult& HighLevelResult = RunAStar(BaseGrid.Get(), PathRequest);
		
		if(HighLevelResult.PathfindingOutCome == EPathfindingOutCome::Failed) return;
		
		UE::Tasks::FTask RunHPAStar = UE::Tasks::Launch(UE_SOURCE_LOCATION,[HighLevelResult, InGridCollection, this, PathRequest, AgentWhoRequestedPath]
		{
			//The first path from startpoint to endpoint
			FHpaStarPath FinalPath;
			FGridCell TargetCell;
			const int32 StartingHighLevelCell = HighLevelResult.CellsToPass[0];
			const int32 NextHighLevelCell = HighLevelResult.CellsToPass[1];
			const FCluster& StartCluster = InGridCollection->GetMutableClusters()[StartingHighLevelCell];
			const FGrid& StartClusterSubGrid = StartCluster.SubGrid;
			const FGridCell& StartingCell = StartClusterSubGrid.GetCellAtPosition(PathRequest.StartLocation);

			
			if(HighLevelResult.CellsToPass.Num() > 1)
			{
				const FClusterConnection& StartEntrance = StartCluster.GetClusterConnection(NextHighLevelCell);
				TargetCell = StartClusterSubGrid.GetCells()[StartEntrance.CellIndexEntranceConnection];
			}
			else
			{
				TargetCell = StartClusterSubGrid.GetCellAtPosition(PathRequest.TargetLocation);
			}
			
			const FAStarPathRequest StartPathRequest(StartingHighLevelCell, StartingCell.CellIndex, TargetCell.CellIndex);
			
			const FAStarPathfindingResult& StartCellResult = RunAStar(&StartClusterSubGrid, StartPathRequest);
			
			for (const int32 CellIndex : StartCellResult.CellsToPass)
			{
				FinalPath.PositionsToWalk.Emplace(StartClusterSubGrid.GetCells()[CellIndex].CellIndex);
			}
			
			// Combine all high level paths
			// skip the first entry and the last entry
			 for (int32 i = 1; i < HighLevelResult.CellsToPass.Num() - 1; ++i)
			 {
			 	const int32 HighLevelCellIndex = HighLevelResult.CellsToPass[i];
			 	const int32 NextHighLevelCellIndex = HighLevelResult.CellsToPass[i + 1];
			 	const FCluster& HighLevelCluster = InGridCollection->GetMutableClusters()[HighLevelCellIndex];
			 	const FGrid& ClusterSubgrid = HighLevelCluster.SubGrid;
			 	const FClusterConnection& OutConnection = HighLevelCluster.GetClusterConnection(NextHighLevelCellIndex);
				const TArray<int32>& CachedPath = OutConnection.CachedPaths[HighLevelCellIndex].CachedPath;
			 	
			 	for (const int32 CachedIndex : CachedPath)
			 	{
			 		FinalPath.PositionsToWalk.Emplace(ClusterSubgrid.GetCells()[CachedIndex].Center);
			 	}
			 };
			
			 //Perform the last cell pathfinding
			 FAStarPathfindingResult EndResult;
			 const int32 PreviousHighLevelCell = HighLevelResult.CellsToPass[HighLevelResult.CellsToPass.Num() - 2];
			 const int32 EndHighLevelCell = HighLevelResult.CellsToPass.Last();
			 const FCluster& PreviousCluster = InGridCollection->GetMutableClusters()[PreviousHighLevelCell];
			 const FCluster& EndCluster = InGridCollection->GetMutableClusters()[EndHighLevelCell];
			 const FGrid& EndClusterSubGrid = EndCluster.SubGrid;
			 //The exit of our previous cell is the start of our last path
			 const FGridCell& EndHighLevelStartCell = EndCluster.SubGrid.GetCells()[PreviousCluster.GetClusterConnection(EndHighLevelCell).CellIndexExitConnection];
			 const FGridCell& EndHighLevelTargetCell = EndClusterSubGrid.GetCellAtPosition(PathRequest.TargetLocation);
			
			 IPathingAgent* PathingAgent = Cast<IPathingAgent>(AgentWhoRequestedPath);
			 const FAStarPathRequest EndCellRequest{EndHighLevelCell, EndHighLevelStartCell.CellIndex, EndHighLevelTargetCell.CellIndex};
			 const FAStarPathfindingResult& EndCellResult = RunAStar(&EndCluster.SubGrid, EndCellRequest);
			
			 if(!PathingAgent) return;
			 if(AgentWhoRequestedPath.IsValid())
			 {
			 	for (int32 i = 0; i < EndCellResult.CellsToPass.Num() - 1; ++i)
			 	{
			 		const int32 EndCellIndex = EndCellResult.CellsToPass[i];
			 		const FVector PositionToAdd = InGridCollection->GetMutableClusters()[EndCellResult.HighLevelGridCellIndex].SubGrid.GetCells()[EndCellIndex].Center;
			 		FinalPath.PositionsToWalk.Emplace(PositionToAdd);
			 	}
			 	
			 	FinalPath.PositionsToWalk.Emplace(PathRequest.TargetLocation);
			 	PathingAgent->RecieveHPAStarPath(FinalPath);
			 }
		});
		UE::Tasks::AddNested(RunHPAStar);
	});
}

void UPathFindingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(!GridCollection)
	{
		
		return;
	};
	
	BaseGrid = MakeUnique<FGrid>(GridCollection->GetMutableBaseGrid());
}

FAStarPathfindingResult UPathFindingSubsystem::RunAStar(const FGrid* GridToRunAStarOn, const FAStarPathRequest& Request)
{
	int32 FromIndex = Request.FromIndex;
	int32 ToIndex = Request.ToIndex;

	//Priority queue we alway pop the last element of this array so we need to sort reverse meaning the best node needs to be the
	//last one in this array
	TArray<TSharedPtr<FAStarNode>> OpenNodes;
	//A set that contains every node that weve already calculated a score for or weve already looked at
	TSet<int32> ClosedNodes;
	FAStarPathfindingResult Result;
	Result.HighLevelGridCellIndex = Request.HighLevelGridCell;
	Result.FromCellIndex = Request.FromIndex;
	
	const FGridCell& StartCell = GridToRunAStarOn->GetCells()[FromIndex];
	const FGridCell& TargetCell = GridToRunAStarOn->GetCells()[ToIndex];
	
	//If we start at end the exit
	if(FromIndex == ToIndex)
	{
		Result.CellsToPass.Add(FromIndex);
		return Result;
	}
	
	OpenNodes.Add(MakeShared<FAStarNode>( 0, FromIndex, nullptr));
	
	while (!OpenNodes.IsEmpty())
	{
		const TSharedPtr<FAStarNode>& CurrentNode = OpenNodes.Pop();
		const FGridCell& CurrentCell = GridToRunAStarOn->GetCells()[CurrentNode->CellIndex];

		if(CurrentNode->CellIndex == ToIndex)
		{
			//Create the result by walking back the path and reversing it
			// //Also push the very first cell to the complete path
			// Result.CellsToPass.Emplace(StartCell.CellIndex);
			WalkBackPath(CurrentNode,Result);
			break;
		}
		for (const auto& OpenNode : GetWeightedNeighboringNodes(StartCell, CurrentCell, CurrentNode, TargetCell, GridToRunAStarOn, ClosedNodes))
		{
			OpenNodes.Emplace(OpenNode);
			ClosedNodes.Emplace(OpenNode->CellIndex);
		}

		OpenNodes.Sort([](const TSharedPtr<FAStarNode>& A, const TSharedPtr<FAStarNode>& B)
		{
			return A->Score > B->Score;
		});
	
		ClosedNodes.Add(CurrentNode->CellIndex);
	}
	
	return Result;
}

void UPathFindingSubsystem::WalkBackPath(const TSharedPtr<FAStarNode>& FinalNode, FAStarPathfindingResult& OutResult)
{
	TSharedPtr<FAStarNode> CurrentNode = FinalNode;

	while (CurrentNode)
	{
		if(!CurrentNode) continue;
		
		OutResult.CellsToPass.Emplace(CurrentNode->CellIndex);
		CurrentNode = CurrentNode->FromNode;
	}
	
	Algo::Reverse(OutResult.CellsToPass);
	OutResult.PathfindingOutCome = EPathfindingOutCome::Success;
}

TArray<TSharedPtr<FAStarNode>> UPathFindingSubsystem::GetWeightedNeighboringNodes(
	const FGridCell& StartCell,
	const FGridCell& CurrentCell,
	const TSharedPtr<FAStarNode>& CurrentNode,
	const FGridCell& TargetCell,
	const FGrid* Grid,
	TSet<int32>& ClosedList,
	bool bRunsOnHighLevel,
	const TWeakObjectPtr<UTDGridCollection> GridCollection)
{
	TArray<TSharedPtr<FAStarNode>> NeighboringNodes;
	const TArray<FGridCell>& NeighboringCells = Grid->GetNeighborsOfCell(CurrentCell);

	for (const FGridCell&  NeighboringCell : NeighboringCells)
	{
		//If the cell is blocked by obstacles buildings etc return
		int32 CellIndex = Grid->GetCellIndex(NeighboringCell);

		if(ClosedList.Contains(CellIndex)) continue;
		if(NeighboringCell.bIsBlocked)
		{
			ClosedList.Add(CellIndex);
			continue;
		}
		if(bRunsOnHighLevel)
		{
			if(!HasPathToNeighboringCell(CurrentCell, GridCollection->GetMutableClusters()[CellIndex], NeighboringCell))
			{
				ClosedList.Add(NeighboringCell.CellIndex);
				continue;
			}
		}
		
		//Calculate squared distance beacuse its cheaper and we dont need a hundred percent accurate results
		const float DistanceToStart = FVector::DistSquared2D(StartCell.Center, NeighboringCell.Center) * 0.1f;
		const float DistanceToEnd = FVector::DistSquared2D(TargetCell.Center, NeighboringCell.Center) * 0.1f;
		
		const float GCost = DistanceToStart;
		const float HCost = DistanceToEnd;
		const float FCost = GCost + HCost;

		NeighboringNodes.Emplace(MakeShared<FAStarNode>(FCost, CellIndex, CurrentNode));
	}
	
	return NeighboringNodes;
}

void UPathFindingSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UPathFindingSubsystem::HasPathToNeighboringCell(const FGridCell& CurrentCell, const FCluster& CurrentCluster, const FGridCell& NeighboringCell)
{
	FClusterConnection Out;
	return CurrentCluster.TryGetClusterConnection(CurrentCell.CellIndex, Out);
}

