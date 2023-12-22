#pragma once

#include "CoreMinimal.h"
#include "AStarPathRequest.h"

struct FGrid;

class TDPATHFINDING_API AStarThread final : public FRunnable
{
public:
	AStarThread(const FGrid& InGrid);
	~AStarThread();

	void PushTaskToThread(const TSharedRef<FAStarPathRequest> InPathRequest);
	
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	bool bIsRecievingTasks = false;
	bool bIsRunning = true;
	
	FRunnableThread* AStarThreadHandle = nullptr;
	FCriticalSection Mutex;
	TQueue<TSharedRef<FAStarPathRequest>, EQueueMode::Mpsc> QueuedTasksForExecution;
	TQueue<TSharedRef<FAStarPathRequest>> TasksToExecute;

	TSharedRef<FAStarPathRequest> CurrentPathRequest;

	TUniquePtr<FGrid> BaseGrid = nullptr;
	void OnTaskFinished();
};
