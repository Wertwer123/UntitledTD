#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PathingAgent.generated.h"

struct FHpaStarPath;
struct FAStarPathfindingResult;
// This class does not need to be modified.
UINTERFACE()
class UPathingAgent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDPATHFINDING_API IPathingAgent
{
	GENERATED_BODY()

public:
	virtual void RecievePath(FAStarPathfindingResult& PathfindingResult){}
	virtual void RecieveHPAStarPath(FHpaStarPath& HPAStarPathfindingResult){}
	virtual void MoveOnPath(){}

};
