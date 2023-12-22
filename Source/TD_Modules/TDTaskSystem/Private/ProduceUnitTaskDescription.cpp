#include "ProduceUnitTaskDescription.h"
#include "Math/Vector.h"
#include "BuildingActor.h"

DEFINE_LOG_CATEGORY(LogTasks)

void FProduceUnitTask::InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture, const bool bIsTaskQueable)
{
	FTDTask::InitTask(InExecutor, TaskDisplayTexture, bIsTaskQueable);
	TaskIdentifier = ETaskIdentifier::ProduceUnit;
}

void FProduceUnitTask::ExecuteTask(const float DeltaSeconds)
{
	//If we cant execute the task then just cancel it
	if(!CanExecuteTask())
	{
		OnTaskCompleted.Broadcast();
	}
	
	RunTime += DeltaSeconds;

	if(IsTaskFinished())
	{
		if(const TObjectPtr<ABuildingActor> BuildingActor = Cast<ABuildingActor>(Executor))
		{
			UE_LOG(LogTasks, Log, TEXT("We produced a unit hurray"))
			const FVector SpawnLocation = BuildingActor->UnitMovePoint.GetUnitMovePointWorld(BuildingActor->GetActorTransform());
			const FRotator SpawnRotation = FRotator::ZeroRotator;
			AUnit* SpawnedUnit = Executor->GetWorld()->SpawnActor<AUnit>(UnitData->GetUnitActorClass(), SpawnLocation, SpawnRotation);
			SpawnedUnit->SetupUnit(UnitData);
		}
		
		//Needs to be bound otherwise crash
		OnTaskCompleted.Broadcast();
	}
}

bool FProduceUnitTask::IsTaskFinished()
{
	return  RunTime > ProdutionTime;
}

TArray<FName> UProduceUnitTaskDescription::GetAllUnits()
{
	if(!UnitDataBase) return {"NONE"};
	
	return UnitDataBase->GetAllUnitNames();
}

TSharedPtr<FTDTask> UProduceUnitTaskDescription::CreateTaskObject(const TObjectPtr<UObject> Executor)
{
	TSharedPtr<FProduceUnitTask> ProduceUnitTask = MakeShared<FProduceUnitTask>();
	ProduceUnitTask->InitTask(Executor, TaskDisplay, bIsQueable);
	ProduceUnitTask->UnitData = UnitDataBase->GetUnit(UnitToProduce);
	ProduceUnitTask->ProdutionTime = ProductionTime;
	
	return ProduceUnitTask;
}
