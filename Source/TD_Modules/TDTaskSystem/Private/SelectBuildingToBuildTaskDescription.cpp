// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectBuildingToBuildTaskDescription.h"

#include "BuildingSubsystem.h"

void FSelectBuildingToBuildTask::InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture, const bool bIsTaskQueable)
{
	FTDTask::InitTask(InExecutor, TaskDisplayTexture, bIsQueueable);
	TaskIdentifier = ETaskIdentifier::SelectBuildingToBuild;
}

void FSelectBuildingToBuildTask::ExecuteTask(const float DeltaSeconds)
{
	if(!CanExecuteTask()) return;
	
	const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = Executor->GetWorld()->GetSubsystem<UBuildingSubsystem>();

	BuildingSubsystem->SetCurrentlySelectedBuilding(BuildingToSelect);
}

bool FSelectBuildingToBuildTask::CanExecuteTask()
{
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = Executor->GetWorld()->GetSubsystem<UBuildingSubsystem>();

	return  BuildingSubsystem->IsBuildingModeActive();
}

TArray<FName> USelectBuildingToBuildTaskDescription::GetAllBuildingNamesInDataBase()
{
	if(!BuildingDataBase) return {FName("NONE")};

	return BuildingDataBase->GetAllBuildingNames();
}

TSharedPtr<FTDTask> USelectBuildingToBuildTaskDescription::CreateTaskObject(const TObjectPtr<UObject> Executor)
{
	TSharedPtr<FSelectBuildingToBuildTask> NewSelectBuildingTask = MakeShared<FSelectBuildingToBuildTask>();
	NewSelectBuildingTask->InitTask(Executor, TaskDisplay, bIsQueable);
	NewSelectBuildingTask->BuildingToSelect = BuildingToSelect;

	return  NewSelectBuildingTask;
}
