// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingData.h"
#include "ResourceBuildingData.generated.h"

UCLASS()
class BUILDING_API UResourceBuildingData : public UBuildingData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category= "ResourceBuildingData")
	float ResourceGenerationRate = 0.2f;
};
