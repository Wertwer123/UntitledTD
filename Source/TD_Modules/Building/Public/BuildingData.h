#pragma once
#include "CoreMinimal.h"
#include "TDStatsComponent.h"
#include "BuildingActor.h"
#include "BuildingGhost.h"
#include "Engine/DataAsset.h"
#include "BuildingData.generated.h"

UCLASS()
class BUILDING_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category= "BuildingData")
	TSubclassOf<ABuildingGhost> BuildingGhost;

	UPROPERTY(EditAnywhere, Category= "BuildingData")
	TSubclassOf<ABuildingActor> BuildingToPlace;

	UPROPERTY(EditAnywhere, Category= "BuildingData|Tasks")
	TArray<TObjectPtr<UTaskDescription>> TaskThisBuildingIsAbleToExecute;

	UPROPERTY(EditAnywhere, Category= "BuildingData|Stats")
	TSet<FTDOffensiveStat> OffensiveStats{};

	UPROPERTY(EditAnywhere, Category= "BuildingData|Stats")
	TSet<FTDDefensiveStat> DefensiveStats{};
	
	UTexture2D* GetBuildingSprite() const {return BuildingSprite;}
	FName GetBuildingName() const {return BuildingName;}


private:
	UPROPERTY(EditAnywhere, Category= "BuildingData")
	FName BuildingName = FName();

	UPROPERTY(EditAnywhere, Category= "BuildingData")
	UTexture2D* BuildingSprite = nullptr;
};
