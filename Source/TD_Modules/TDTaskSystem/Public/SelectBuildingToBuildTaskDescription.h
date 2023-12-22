// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingDataBase.h"
#include "TDTaskDescription.h"
#include "SelectBuildingToBuildTaskDescription.generated.h"


USTRUCT()
struct TDTASKSYSTEM_API FSelectBuildingToBuildTask : public FTDTask
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category= "Task")
	FName BuildingToSelect = "";

	virtual void InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture, const bool bIsTaskQueable) override;
	virtual void ExecuteTask(const float DeltaSeconds) override;
	virtual bool CanExecuteTask() override;
};

UCLASS()
class TDTASKSYSTEM_API USelectBuildingToBuildTaskDescription : public UTaskDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(GetOptions = "GetAllBuildingNamesInDataBase"), Category= "Task|Building")
	FName BuildingToSelect = "NONE";
	
	UPROPERTY(EditAnywhere, Category= "Task|Building")
	TObjectPtr<UBuildingDataBase> BuildingDataBase;

	UFUNCTION()
	TArray<FName> GetAllBuildingNamesInDataBase();
	
	virtual TSharedPtr<FTDTask> CreateTaskObject(const TObjectPtr<UObject> Executor) override;
};
