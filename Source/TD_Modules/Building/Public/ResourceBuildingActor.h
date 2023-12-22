#pragma once
#include "TimerManager.h"
#include "CoreMinimal.h"
#include "BuildingActor.h"
#include "EResourceType.h"
#include "ResourceBuildingActor.generated.h"

UCLASS()
class BUILDING_API AResourceBuildingActor : public ABuildingActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category= "DebugInfor")
	float ResourceGenerationRate = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category= "ResourceBuilding")
	EResourceType TypeOfResourceToProduce = EResourceType::AlienBlood;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	FTimerDelegate TimerDelegate;

	UFUNCTION()
	void GenerateResource();
	
	virtual void SetupBuilding(const TObjectPtr<const UBuildingData> BuildingData) override;
};
