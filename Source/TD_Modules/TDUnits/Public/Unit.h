#pragma once

#include "CoreMinimal.h"
#include "AStarPathRequest.h"
#include "Damageable.h"
#include "PathingAgent.h"
#include "Selectable.h"
#include "TaskQueueComponent.h"
#include "TaskReciever.h"
#include "TDStatsComponent.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

struct FAStarPathfindingResult;
class UUnitData;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUnitSelected, const FName&, NameOfSelectedUnit);

UCLASS()
class TDUNITS_API AUnit : public AActor, public ITaskReciever, public IDamageable, public ISelectable, public IPathingAgent
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

	UPROPERTY()
	FHpaStarPath PathToFollow;

	UPROPERTY()
	int32 CurrentPathIndex = 0;
	
	virtual void BeginPlay() override;
	virtual void RecieveTask(const TSharedPtr<FTDTask> RecievedTask) override;
	virtual void RecieveDamage(const ETDOffensiveStatType TypeOfDmgToDeal, const int32 Dmg) override;
	virtual void RecieveHPAStarPath(FHpaStarPath& HPAStarPathfindingResult) override;
	virtual void MoveOnPath() override;
	virtual void OnSelect() override;
	virtual void OnDeselect() override;

	bool bIsSelected = false;
public:

	virtual void Tick(float DeltaTime) override;
	FName GetUnitName() const {return UnitName;}
	void SetupUnit(const TObjectPtr<const UUnitData>& UnitData);
};
