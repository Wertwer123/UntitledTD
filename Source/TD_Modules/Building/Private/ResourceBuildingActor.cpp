// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/ResourceBuildingActor.h"

#include "ResourceBuildingData.h"

void AResourceBuildingActor::SetupBuilding()
{
	if(const TObjectPtr<UResourceBuildingData> ResourceBuildingData = Cast<UResourceBuildingData>(BuildingData))
	{
		
	}
	else
	{
		ensureMsgf(ResourceBuildingData, TEXT("Youd dindt assign a resourcebuilding data to a respurce building"));
	}
	
	Super::SetupBuilding();
}
