#pragma once

#include "CoreMinimal.h"
#include "TDTaskDescription.h"
#include "PathingAgent.h"
#include "MoveUnitTaskDescription.generated.h"

USTRUCT()
struct TDTASKSYSTEM_API FMoveUnitTask : public FTDTask
{
	GENERATED_BODY()

	FMoveUnitTask(){}
	FMoveUnitTask(const TArray<int32>& CellsToWalk) :
		CellsToWalk(CellsToWalk){}
	
	UPROPERTY()
	TArray<int32> CellsToWalk;

	IPathingAgent* PathingAgent;
	
	virtual void InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture, const bool bIsTaskQueable) override;
	virtual void ExecuteTask(const float DeltaSeconds) override;
};

UCLASS()
class TDTASKSYSTEM_API UMoveUnitTaskDescription : public UTaskDescription
{
	GENERATED_BODY()
};
