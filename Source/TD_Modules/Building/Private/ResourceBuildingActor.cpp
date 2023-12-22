// Fill out your copyright notice in the Description page of Project Settings.
#include "ResourceBuildingActor.h"

#include "ResourceBuildingData.h"
#include "TimerManager.h"

void AResourceBuildingActor::GenerateResource()
{
	//In here make a call to the resource soubsystem where we add the resource and update the UI Accordingly in the hud
	UE_LOG(LogBuildings, Log, TEXT("Generated a resource %s"), *UEnum::GetValueAsString(TypeOfResourceToProduce))
}

void AResourceBuildingActor::SetupBuilding(const TObjectPtr<const UBuildingData> BuildingData)
{
	Super::SetupBuilding(BuildingData);

	if(TObjectPtr<UResourceBuildingData> ResourceBuildingData = Cast<UResourceBuildingData>(BuildingData))
	{
		ResourceGenerationRate = ResourceBuildingData->ResourceGenerationRate;
		
		TimerDelegate.BindUObject(this, &AResourceBuildingActor::GenerateResource);
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, ResourceGenerationRate, true);
	}
	else
	{
		ensureMsgf(false, TEXT("You didnt assign a resource buildingData to a resource building kekw"));
	}
}
