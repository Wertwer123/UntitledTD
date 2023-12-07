// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceBuildingActor.h"
#include "Task_Description.h"
#include "HQBuilding.generated.h"

UCLASS()
class BUILDING_API AHQBuilding : public AResourceBuildingActor
{
	GENERATED_BODY()

public:
	AHQBuilding();

private:
	UPROPERTY()
	FTDTaskQueue TaskQueue;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
