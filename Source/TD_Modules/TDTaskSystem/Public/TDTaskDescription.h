#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "Templates/SharedPointer.h"
#include "TDTaskDescription.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTasks, All, Log)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskFinished);

UENUM(BlueprintType)
enum class ETaskIdentifier
{
	ProduceUnit,
	Move,
	Attack,
	Patrol,
	Follow,
	SelectBuildingToBuild
};

USTRUCT()
struct FTDTask
{
	GENERATED_BODY()
	virtual ~FTDTask() = default;
	
	FOnTaskFinished OnTaskCompleted;

	TObjectPtr<UTexture2D> GetTaskDisplay() const { return TaskDisplay;}
	///No Base functionality meant to be overriden
	virtual void ExecuteTask(const float DeltaSeconds = 0){}
	virtual void InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture, const bool bIsTaskQueable)
	{
		TaskDisplay = TaskDisplayTexture;
		bIsQueueable = bIsTaskQueable;
		Executor = InExecutor;
	};
	//Maybe implement something like a fcondition class so that we can generically and easily check if conditions for tasks are met 
	virtual bool CanExecuteTask(){return true;}
	virtual  bool IsQueable(){return bIsQueueable;}
	
protected:
	///This is used the make easier and faster and to easily identify tasks
	UPROPERTY()
	ETaskIdentifier TaskIdentifier = ETaskIdentifier::ProduceUnit;

	UPROPERTY()
	TObjectPtr<UTexture2D> TaskDisplay = nullptr;
	
	UPROPERTY()
	TObjectPtr<UObject> Executor = nullptr;

	UPROPERTY()
	bool bIsQueueable = true;
	
	virtual bool IsTaskFinished(){return true;}
};

UCLASS()
class TDTASKSYSTEM_API UTaskDescription : public UDataAsset
{
public:

	TObjectPtr<UTexture2D> GetTaskDisplay() const { return TaskDisplay;}
	///Creates the task object and initializes its values
	virtual TSharedPtr<FTDTask> CreateTaskObject(const TObjectPtr<UObject> Executor);

protected:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category= "Task")
	bool bIsQueable = true;
	
	UPROPERTY(EditAnywhere, Category= "Task")
	TObjectPtr<UTexture2D> TaskDisplay = nullptr;
};
