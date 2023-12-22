// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskQueueComponent.h"


// Sets default values for this component's properties
UTaskQueueComponent::UTaskQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTaskQueueComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UTaskQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ProcessCurrentTask(DeltaTime);
}


void UTaskQueueComponent::PushTaskToQueue(const TSharedPtr<FTDTask>& NewTask)
{
	NewTask->OnTaskCompleted.AddDynamic(this, &UTaskQueueComponent::ChangeTaskAfterFinishedTask);
	CurrentTasks.Insert(NewTask, 0);
	OnTaskAdded.Broadcast(*NewTask);
	//If we dont have a assigned task we instantly take this one other wise we just add it to the queue
	TryChangeCurrentTask();
	
}

void UTaskQueueComponent::ProcessCurrentTask(const float DeltaSeconds) const
{
	if(!CurrentTask) return;
	
	CurrentTask->ExecuteTask(DeltaSeconds);
}

void UTaskQueueComponent::ChangeTaskAfterFinishedTask()
{
	CurrentTask.Reset();
	
	if(CurrentTasks.IsEmpty()) return;
	
	CurrentTask = CurrentTasks.Pop();
}

bool UTaskQueueComponent::TryChangeCurrentTask()
{
	if(CurrentTasks.IsEmpty()) return false;
	if(CurrentTask) return false;
	
	CurrentTask.Reset();
	CurrentTask = CurrentTasks.Pop();
	
	return true;
}

