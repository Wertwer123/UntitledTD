// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingSystemSettings.generated.h"

UCLASS()
class BUILDING_API UBuildingSystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category= BuildingSettings)
	TEnumAsByte<ECollisionChannel> BuildableTraceChannel;

	UPROPERTY(EditAnywhere, Category= BuildingSettings)
	TEnumAsByte<ECollisionChannel> NonBuildableTraceChannel;
	
};
