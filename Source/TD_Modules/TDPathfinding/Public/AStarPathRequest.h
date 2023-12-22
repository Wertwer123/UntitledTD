#pragma once

#include "CoreMinimal.h"
#include "TDGridCollection.h"
#include "AStarPathRequest.generated.h"

struct FGrid;

USTRUCT()
struct TDPATHFINDING_API FAStarPathRequest
{
	GENERATED_BODY();
	
public:
	FAStarPathRequest(){};
	FAStarPathRequest(
		const int32 FromCellIndex,
		const int32 TargetCellIndex) :
	FromIndex(FromCellIndex),
	ToIndex(TargetCellIndex){}

	int32 FromIndex = 0;
	int32 ToIndex = 0;
};
