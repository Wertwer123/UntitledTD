#include "MoveUnitTaskDescription.h"

void FMoveUnitTask::InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture,
                             const bool bIsTaskQueable)
{
	FTDTask::InitTask(InExecutor, TaskDisplayTexture, bIsTaskQueable);

	// PathingAgent = Cast<IPathingAgent>(InExecutor);
}

void FMoveUnitTask::ExecuteTask(const float DeltaSeconds)
{
	FTDTask::ExecuteTask(DeltaSeconds);
}
