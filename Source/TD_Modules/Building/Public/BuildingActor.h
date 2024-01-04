#pragma once
#include "CoreMinimal.h"
#include "Damageable.h"
#include "FUnitCollectionPoint.h"
#include "Selectable.h"
#include "TaskReciever.h"
#include "TDStatsComponent.h"
#include "BuildingActor.generated.h"

class UTaskQueueComponent;
class UTaskDescription;
class UBuildingData;

DECLARE_LOG_CATEGORY_EXTERN(LogBuildings, Log, All)
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBuildingSelected, const FName&, NameOfSelectedBuilding);

UCLASS()
class BUILDING_API ABuildingActor : public AActor, public ISelectable, public ITaskReciever, public IDamageable
{
	GENERATED_BODY()

public:
	ABuildingActor();

	///This point doesnt get used in every building but it
	///needs to sit here to easily access it from the building bas
	UPROPERTY(EditAnywhere, Category= "Building")
	FUnitCollectionPoint UnitMovePoint;
	
	virtual void OnSelect() override;
	virtual void OnDeselect() override;
	virtual void RecieveDamage(const ETDOffensiveStatType TypeOfDmgToDeal, const int32 Dmg) override;
	virtual void RecieveTask(const TSharedPtr<FTDTask> RecievedTask) override;
	virtual void SetupBuilding(const TObjectPtr<const UBuildingData> BuildingData);
	inline static FOnBuildingSelected OnBuildingSelected;

	TObjectPtr<UTDStatsComponent> GetStats() const {return  StatsComponent;}
	TObjectPtr<UTaskQueueComponent> GetTaskQueue(){return TaskQueueComponent;}
	
protected:
	UPROPERTY(VisibleAnywhere, Category= "Building")
	FName BuildingName = "NONE";
	
	UPROPERTY(VisibleAnywhere, Category= "Tasks")
	TArray<TObjectPtr<UTaskDescription>> TaskThisBuildingIsAbleToExecute;

	UPROPERTY(EditDefaultsOnly, Category= "BuildingDisplay")
	TObjectPtr<UStaticMeshComponent> SMC = nullptr;

	UPROPERTY(VisibleAnywhere, Category= "Building")
	TObjectPtr<UTDStatsComponent> StatsComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category= "Building")
	TObjectPtr<UTaskQueueComponent> TaskQueueComponent = nullptr;
	
};
