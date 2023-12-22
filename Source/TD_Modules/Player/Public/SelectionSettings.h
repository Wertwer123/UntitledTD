// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SelectionSettings.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_API USelectionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category= "Selection")
	TEnumAsByte<ECollisionChannel> SelectableCollisionChannel;
};
