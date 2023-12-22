#include "PathFindingSubsystem.h"

void UPathFindingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	StartAStarThread();
}

void UPathFindingSubsystem::RequestAStarPath()
{
	AStarWorker->PushTaskToThread(MakeShared<FAStarPathRequest>(10, 20));
}

void UPathFindingSubsystem::Deinitialize()
{
	Super::Deinitialize();

	StopAStarThread();
}

void UPathFindingSubsystem::StartAStarThread()
{
	AStarWorker = new AStarThread(GridCollection->GetBaseGrid());
}

void UPathFindingSubsystem::StopAStarThread()
{
	delete AStarWorker;
}
