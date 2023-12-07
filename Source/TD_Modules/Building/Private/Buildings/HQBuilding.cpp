#include "HQBuilding.h"
#include "Task_Description.h"

// Sets default values
AHQBuilding::AHQBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHQBuilding::BeginPlay()
{
	Super::BeginPlay();

	for (const TObjectPtr<UTaskDescription>& TaskDescription : TaskThisBuildingIsAbleToExecute)
	{
		TaskQueue.PushTaskToQueue(TaskDescription->CreateTaskObject(this));
	}
}

void AHQBuilding::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TaskQueue.ProcessCurrentTask(DeltaSeconds);
}

