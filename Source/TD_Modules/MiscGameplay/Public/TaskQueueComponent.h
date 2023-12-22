#pragma once
#include "CoreMinimal.h"
#include "TDTaskDescription.h"
#include "TaskQueueComponent.generated.h"

//These refs need to be handled like const refs but we wouldnt be able to unbind from the events if they are const
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskAdded, FTDTask&, AddedTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskRemoved, FTDTask&, RemovedTask);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MISCGAMEPLAY_API UTaskQueueComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UTaskQueueComponent();

	UPROPERTY()
	FOnTaskAdded OnTaskAdded;

	UPROPERTY()
	FOnTaskRemoved OnTaskRemoved;
	
	void PushTaskToQueue(const TSharedPtr<FTDTask>& NewTask);
	void ProcessCurrentTask(const float DeltaSeconds) const;

	UFUNCTION()
	void ChangeTaskAfterFinishedTask();
	
	const TArray<TSharedPtr<FTDTask>>& GetCurrentTasks() const {return CurrentTasks;}
private:

	//TODO implement logic for exchanging the last task in the queue or just blocking with a message if we cant accept any more tasks
	UPROPERTY(EditAnywhere, Category= "TaskQueueSettings")
	int32 MaxAmountOfTasks = 10;
	
	TSharedPtr<FTDTask> CurrentTask = nullptr;
	TArray<TSharedPtr<FTDTask>> CurrentTasks;

	///Tries to change to a new task 
	bool TryChangeCurrentTask();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
