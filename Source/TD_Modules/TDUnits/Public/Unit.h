#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "Selectable.h"
#include "TaskQueueComponent.h"
#include "TaskReciever.h"
#include "TDStatsComponent.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

class UUnitData;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUnitSelected, const FName&, NameOfSelectedUnit);

UCLASS()
class TDUNITS_API AUnit : public AActor, public ITaskReciever, public IDamageable, public ISelectable
{
	GENERATED_BODY()

public:

	AUnit();

	inline static FOnUnitSelected OnUnitSelected;
	
protected:
	UPROPERTY(VisibleAnywhere, Category= UnitDisplay)
	FName UnitName = "";
	
	UPROPERTY(EditDefaultsOnly, Category= UnitDisplay)
	TObjectPtr<UStaticMeshComponent> SMC = nullptr;

	UPROPERTY(VisibleAnywhere, Category= "Unit")
	TObjectPtr<UTDStatsComponent> StatsComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category= "Unit")
	TObjectPtr<UTaskQueueComponent> TaskQueueComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "Unit")
	TArray<TObjectPtr<UTaskDescription>> TaskThisUnitIsAbleToExecute;
	
	virtual void BeginPlay() override;
	virtual void RecieveTask(const TSharedPtr<FTDTask> RecievedTask) override;
	virtual void RecieveDamage(const ETDOffensiveStatType TypeOfDmgToDeal, const int32 Dmg) override;
	virtual void OnSelect() override;

public:

	virtual void Tick(float DeltaTime) override;
	FName GetUnitName() const {return UnitName;}
	void SetupUnit(const TObjectPtr<const UUnitData>& UnitData);
};
