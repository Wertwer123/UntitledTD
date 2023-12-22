#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "Engine/DataAsset.h"
#include "UnitData.generated.h"

class UTaskDescription;
struct FTDOffensiveStat;
struct FTDDefensiveStat;

UCLASS()
class TDUNITS_API UUnitData : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category= "Unit")
	FName UnitName = "";

	UPROPERTY(EditDefaultsOnly, Category= "Unit")
	TArray<TObjectPtr<UTaskDescription>> TaskThisUnitIsAbleToExecute;
	
	UPROPERTY(EditAnywhere, Category= "Unit")
	TSet<FTDOffensiveStat> OffensiveStats{};

	UPROPERTY(EditAnywhere, Category= "Unit")
	TSet<FTDDefensiveStat> DefensiveStats{};

	UPROPERTY(EditAnywhere, Category= "BuildingData")
	TSubclassOf<AUnit> ActorToSpawn;

public:

	FName GetUnitName() const {return UnitName;}
	const TArray<TObjectPtr<UTaskDescription>>& GetTasksThisUnitIsAbleToExecute() const {return TaskThisUnitIsAbleToExecute;}
	const TSubclassOf<AUnit>& GetUnitActorClass() const {return  ActorToSpawn;}
	const TSet<FTDOffensiveStat>& GetOffensiveStats() const {return OffensiveStats;}
	const TSet<FTDDefensiveStat>& GetDefensiveStats() const {return DefensiveStats;}
};
