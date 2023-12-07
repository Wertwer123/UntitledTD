#include "ProduceUnitTaskDescription.h"

DEFINE_LOG_CATEGORY(LogTasks)

void FProduceUnitTask::InitTask(const TObjectPtr<AActor> InExecutor)
{
	FTDTask::InitTask(InExecutor);
	TaskIdentifier = ETaskIdentifier::ProduceUnit;
}

void FProduceUnitTask::ExecuteTask(const float DeltaSeconds)
{
	RunTime += DeltaSeconds;

	if(IsTaskFinished())
	{
		UE_LOG(LogTasks, Log, TEXT("We produced a unit hurray"))
		//TODO Later exchange with the unit spawn point and assign a move task to the unit to the collection point
		const FVector SpawnLocation = FVector::Zero();
		Executor->GetWorld()->SpawnActor(UnitToSpawn, &SpawnLocation);
		//Needs to be bound otherwise crash
		OnTaskCompleted.Execute();
	}
}

bool FProduceUnitTask::IsTaskFinished()
{
	return  RunTime > ProdutionTime;
}

TSharedPtr<FTDTask> UProduceUnitTaskDescription::CreateTaskObject(const TObjectPtr<AActor> Executor)
{
	TSharedPtr<FProduceUnitTask> ProduceUnitTask = MakeShared<FProduceUnitTask>();
	
	return ProduceUnitTask;
}
