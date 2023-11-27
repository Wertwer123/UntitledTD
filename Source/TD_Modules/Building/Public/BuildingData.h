// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingActor.h"
#include "BuildingGhost.h"
#include "Engine/DataAsset.h"
#include "BuildingData.generated.h"

/**
 * 
 */
UCLASS()
class BUILDING_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:
	FName GetBuildingName() const {return BuildingName;}
	UTexture2D* GetBuildingSprite() const {return BuildingSprite;}

	UPROPERTY(EditAnywhere, Category= BuildingData)
	TSubclassOf<ABuildingGhost> BuildingGhost;

	UPROPERTY(EditAnywhere, Category= BuildingData)
	TSubclassOf<ABuildingActor> BuildingToPlace;

private:
	UPROPERTY(EditAnywhere, Category= BuildingData)
	FName BuildingName = FName();

	UPROPERTY(EditAnywhere, Category= BuildingData)
	UTexture2D* BuildingSprite = nullptr;
};
