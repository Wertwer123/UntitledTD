// Fill out your copyright notice in the Description page of Project Settings.
#include "BuildingDataBase.h"
#include "BuildingData.h"

TArray<FName> UBuildingDataBase::GetAllBuildingNames() const
{
	TArray<FName> AllBuildingNames;

	if(BuildingData.IsEmpty()) return AllBuildingNames;
	
	for (const auto* Building : BuildingData)
	{
		AllBuildingNames.Add(Building->GetBuildingName());
	}

	return AllBuildingNames;
}

TObjectPtr<const UBuildingData> UBuildingDataBase::GetBuildingDataByName(const FName& BuildingName)
{
	TObjectPtr<const UBuildingData> FoundBuilding = nullptr;

	for (const UBuildingData* AvailableBuildingData : BuildingData)
	{
		if(AvailableBuildingData->GetBuildingName().IsEqual(BuildingName))
		{
			FoundBuilding = AvailableBuildingData;
		}
	}

	return FoundBuilding;
}

UTexture2D* UBuildingDataBase::GetSpriteOfBuilding(const FName& NameOfBuilding) const
{
	UTexture2D* BuildingSprite = nullptr;
	
	for (const UBuildingData* Building : BuildingData)
	{
		if(Building->GetBuildingName() == NameOfBuilding)
		{
			BuildingSprite = Building->GetBuildingSprite();
		}
	}

	return BuildingSprite;
}

