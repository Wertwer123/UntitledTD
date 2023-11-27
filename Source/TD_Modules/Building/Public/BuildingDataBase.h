// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingData.h"
#include "BuildingDataBase.generated.h"

/**
 * 
 */
UCLASS()
class BUILDING_API UBuildingDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	TArray<FName> GetAllBuildingNames() const;
	TObjectPtr<const UBuildingData> GetBuildingDataByName(const FName& BuildingName);
	UTexture2D* GetSpriteOfBuilding(const FName& NameOfBuilding) const;
	
private:
	
	UPROPERTY(EditAnywhere, Category= BuildingData)
	TArray<UBuildingData*> BuildingData {};
	
};
