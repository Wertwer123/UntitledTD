// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDTaskDescription.h"
#include "ProduceUnitTaskDescription.generated.h"

struct FProduceUnitTask : public  FTDTask
{
	int32 ProdutionTime;
	float RunTime = 0;
	//TODO Exchange later with actual unit class
	TSubclassOf<AActor> UnitToSpawn;

	///Initializes the task with the right default values so that we cant do anything wrong in the editor
	///Because code bigger human 
	virtual void InitTask(const TObjectPtr<AActor> InExecutor) override;
	virtual void ExecuteTask(const float DeltaSeconds = 0) override;
	virtual bool IsTaskFinished() override;
};

UCLASS()
class TASKSYSTEM_API UProduceUnitTaskDescription : public UTaskDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category= "Task|Unit")
	TSubclassOf<AActor> UnitToProduce;

	UPROPERTY(EditAnywhere, Category= "Task|Unit")
	int32 ProductionTime = 2;

	virtual TSharedPtr<FTDTask> CreateTaskObject(const TObjectPtr<AActor> Executor) override;
};
