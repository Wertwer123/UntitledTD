// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarThread.h"


AStarThread::AStarThread(const FGrid& InGrid)
{
	AStarThreadHandle = FRunnableThread::Create(this, TEXT("AStarThread"));
	BaseGrid = MakeUnique<FGrid>(InGrid);
}

AStarThread::~AStarThread()
{
	if(AStarThreadHandle)
	{
		AStarThreadHandle->Kill();
		delete AStarThreadHandle;
	}
}

void AStarThread::PushTaskToThread(const TSharedRef<FAStarPathRequest> InPathRequest)
{
	bIsRecievingTasks = true;
	UE_LOG(LogTemp, Log, TEXT("Enqued a task"))
	QueuedTasksForExecution.Enqueue(InPathRequest);

	bIsRecievingTasks = false;
}

bool AStarThread::Init()
{
	return FRunnable::Init();
}

uint32 AStarThread::Run()
{
	while (bIsRunning)
	{
		if(!bIsRecievingTasks)
		{
			Mutex.Lock();

			TArray<int32> OpenList;
			TArray<int32> ClosedList;
			
			
			UE_LOG(LogTemp, Log, TEXT("Dequed a task"))
			QueuedTasksForExecution.Dequeue(CurrentPathRequest);

			OpenList.Add(CurrentPathRequest->FromIndex);

			while (!OpenList.IsEmpty())
			{
				//Get all neighbours of the cell
			}
			
			Mutex.Unlock();
		}
	}

	return 1;
}

void AStarThread::Stop()
{
	bIsRunning = false;
}

void AStarThread::OnTaskFinished()
{
	
}
