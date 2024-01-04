#pragma once

#include "CoreMinimal.h"
#include "AStarPathRequest.h"
#include "TDGridCollection.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathFindingSubsystem.generated.h"

struct FAStarPathRequest;

UCLASS()
class TDPATHFINDING_API UPathFindingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UTDGridCollection> GridCollection;
	
	TUniquePtr<FGrid> BaseGrid = nullptr;
	
	//Creates an astar path request object 
	FAStarPathRequest CreateRequest(const FVector& StartLocation, const FVector& EndLocation);
	
	//A simple astar request that can be run on any grid 
	void RequestAStarPath(const FAStarPathRequest& PathRequest, const TSharedPtr<FGrid>& GridToRunAStarOn, TWeakObjectPtr<UObject> AgentWhoRequestedPath);
	
	void RequestHPAStarPath(const FAStarPathRequest& PathRequest, TWeakObjectPtr<UObject> AgentWhoRequestedPath);


private:
	
	void OnWorldBeginPlay(UWorld& InWorld) override;
	void WalkBackPath(const TSharedPtr<FAStarNode>& FinalNode, FAStarPathfindingResult& OutResult);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static bool HasPathToNeighboringCell(const FGridCell& CurrentCell, const FCluster& CurrentCluster, const FGridCell& NeighboringCell);
	FAStarPathfindingResult RunAStar(const FGrid* GridToRunAStarOn, const FAStarPathRequest& Request);

	static TArray<TSharedPtr<FAStarNode>> GetWeightedNeighboringNodes(
		const FGridCell& StartCell,
		const FGridCell& CurrentCell,
		const TSharedPtr<FAStarNode>& CurrentNode,
		const FGridCell& TargetCell,
		const FGrid* Grid,
		TSet<int32>& ClosedList,
		bool bRunsOnHighLevel = false,
		const TWeakObjectPtr<UTDGridCollection> GridCollection = nullptr);
};
