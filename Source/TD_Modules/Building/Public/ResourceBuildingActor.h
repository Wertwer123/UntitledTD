// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingActor.h"
#include "ResourceBuildingActor.generated.h"

UCLASS()
class BUILDING_API AResourceBuildingActor : public ABuildingActor
{
	GENERATED_BODY()

protected:
	virtual void SetupBuilding() override;
	virtual void BeginPlay() override;
	
};
