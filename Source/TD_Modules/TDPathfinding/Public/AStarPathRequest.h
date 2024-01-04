#pragma once

#include "CoreMinimal.h"
#include "AStarPathRequest.generated.h"

struct FGrid;

UENUM()
enum class EPathfindingOutCome
{
	Success,
	Failed
};

struct FAStarNode;

struct TDPATHFINDING_API FAStarNode
{
	FAStarNode(){};
	FAStarNode(const int32 CellIndex) :
		CellIndex(CellIndex){};
	//Creates a normal astar node for astar calculation
	FAStarNode(const float Score, const int32 CellIndex, const TSharedPtr<FAStarNode>& InFromNode) :
		Score(Score),
		CellIndex(CellIndex),
		FromNode(InFromNode)
	{
		
		
	}
	
	float Score = 0.0f;
	int32 CellIndex = 0;
	TSharedPtr<FAStarNode> FromNode = nullptr;
};

USTRUCT()
struct TDPATHFINDING_API FAStarPathfindingResult
{
	GENERATED_BODY()

	FAStarPathfindingResult(){};
	FAStarPathfindingResult(
		const int32 InUpperGridCellIndex,
		const EPathfindingOutCome InPathFindingOutcome,
		const TArray<int32>& InCellsToPass) :
	
		HighLevelGridCellIndex(InUpperGridCellIndex),
		PathfindingOutCome(InPathFindingOutcome),
		CellsToPass(InCellsToPass){}

	UPROPERTY()
	int32 HighLevelGridCellIndex = 0;

	UPROPERTY()
	int32 FromCellIndex = 0;
	
	UPROPERTY()
	EPathfindingOutCome PathfindingOutCome = EPathfindingOutCome::Failed;

	UPROPERTY()
	TArray<int32> CellsToPass{};
};

USTRUCT()
struct TDPATHFINDING_API FHpaStarPath
{
	GENERATED_BODY()
	TArray<FVector> PositionsToWalk;
};

//A path request for either an hpastar path reuqest or an ordinary astar request
USTRUCT()
struct TDPATHFINDING_API FAStarPathRequest
{
	GENERATED_BODY();
	
public:
	FAStarPathRequest(){};
	FAStarPathRequest(
		const int32 HighLevelGridCellIndex,
		const int32 FromCellIndex,
		const int32 TargetCellIndex) :

			HighLevelGridCell(HighLevelGridCellIndex),
			FromIndex(FromCellIndex),
			ToIndex(TargetCellIndex){}

	FAStarPathRequest(
	const int32 HighLevelGridCellIndex,
	const int32 FromCellIndex,
	const int32 TargetCellIndex,
	const FVector& StartLocation,
	const FVector& TargetLocation) :

		HighLevelGridCell(HighLevelGridCellIndex),
		FromIndex(FromCellIndex),
		ToIndex(TargetCellIndex),
		StartLocation(StartLocation),
		TargetLocation(TargetLocation){}

	bool operator==(const FAStarPathRequest& Other) const
	{
		return FromIndex == Other.FromIndex && HighLevelGridCell == Other.HighLevelGridCell;
	}
	//This index is needed for lowe level pathfindings to identify in which highlevel cell we have
	//Started the pathfinding
	int32 HighLevelGridCell = 0;
	int32 FromIndex = 0;
	int32 ToIndex = 0;
	FVector StartLocation = FVector::Zero();
	FVector TargetLocation = FVector::Zero();
};
