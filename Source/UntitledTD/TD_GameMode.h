// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingDataBase.h"
#include "GameFramework/GameModeBase.h"
#include "TD_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDTD_API ATD_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category= "Initialization")
	TObjectPtr<UBuildingDataBase> BuildingDataBase = nullptr;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
