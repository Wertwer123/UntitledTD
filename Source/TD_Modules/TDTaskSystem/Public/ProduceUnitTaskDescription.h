#pragma once

#include "CoreMinimal.h"
#include "TDTaskDescription.h"
#include "UnitDataBase.h"
#include "ProduceUnitTaskDescription.generated.h"

USTRUCT()
struct TDTASKSYSTEM_API FProduceUnitTask : public  FTDTask
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 ProdutionTime = 0;
	
	UPROPERTY()
	float RunTime = 0;
	
	UPROPERTY()
	TObjectPtr<const UUnitData> UnitData;
	
	///Initializes the task with the right default values so that we cant do anything wrong in the editor
	///Because code bigger human 
	virtual void InitTask(const TObjectPtr<UObject> InExecutor, const TObjectPtr<UTexture2D> TaskDisplayTexture, const bool bIsTaskQueable) override;
	virtual void ExecuteTask(const float DeltaSeconds = 0) override;
	virtual bool IsTaskFinished() override;
};

UCLASS()
class TDTASKSYSTEM_API UProduceUnitTaskDescription : public UTaskDescription
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=( GetOptions = "GetAllUnits"), Category= "Task|Unit")
	FName UnitToProduce;

	UPROPERTY(EditAnywhere, Category= "Task|Unit")
	int32 ProductionTime = 2;

	UPROPERTY(EditDefaultsOnly, Category= "Task|Unit")
	TObjectPtr<UUnitDataBase> UnitDataBase;

	UFUNCTION()
	TArray<FName> GetAllUnits();
	
	virtual TSharedPtr<FTDTask> CreateTaskObject(const TObjectPtr<UObject> Executor) override;
};
