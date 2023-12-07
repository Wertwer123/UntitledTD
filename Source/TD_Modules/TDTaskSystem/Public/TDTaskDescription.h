// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "Templates/SharedPointer.h"
#include "TDTaskDescription.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTasks, All, Log)
DECLARE_DELEGATE(FOnTaskFinished);

UENUM(BlueprintType)
enum class ETaskIdentifier
{
	ProduceUnit,
	Move,
	Attack,
	Patrol,
	Follow
};

USTRUCT()
struct FTDTask
{
	GENERATED_BODY()
	virtual ~FTDTask() = default;
	
	FOnTaskFinished OnTaskCompleted;
	
	virtual void ExecuteTask(const float DeltaSeconds = 0){}
	virtual void InitTask(const TObjectPtr<AActor> InExecutor){Executor = InExecutor;};

protected:
	///This is used the make easier and faster and to easily identify tasks
	UPROPERTY()
	ETaskIdentifier TaskIdentifier = ETaskIdentifier::ProduceUnit;

	UPROPERTY()
	TObjectPtr<AActor> Executor = nullptr;
	
	virtual bool IsTaskFinished(){return true;}
};

UCLASS()
class TASKSYSTEM_API UTaskDescription : public UDataAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category= "Task")
	bool bIsQueable = true;
	
	UPROPERTY(EditAnywhere, Category= "Task")
	TObjectPtr<UTexture2D> TaskDisplay = nullptr;

public:
	///TODO Find a solution for the task identifier problem 
	virtual TSharedPtr<FTDTask> CreateTaskObject(const TObjectPtr<AActor> Executor){return MakeShared<FTDTask>();};
};
