// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BuildingGhost.generated.h"

UCLASS()
class BUILDING_API ABuildingGhost : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingGhost();

	void SetGhostColour(const FColor& NewColor);
	bool IsColorAlreadySet(const FColor& ColorToCheck);
};
