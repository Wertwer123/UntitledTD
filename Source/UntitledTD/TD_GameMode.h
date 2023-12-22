// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingDataBase.h"
#include "BuildingSystemSettings.h"
#include "SelectionSettings.h"
#include "UnitDataBase.h"
#include "GameFramework/GameModeBase.h"
#include "TD_GameMode.generated.h"

class UTDGridCollection;
/**
 * 
 */
UCLASS()
class UNTITLEDTD_API ATD_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	TObjectPtr<UUnitDataBase>& GetUnitDataBase() {return  UnitDataBase;}
	
private:
	UPROPERTY(EditDefaultsOnly, Category= "Initialization")
	TObjectPtr<UTDGridCollection> GridCollection = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "Initialization")
	TObjectPtr<USelectionSettings> SelectionSettings = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "Initialization")
	TObjectPtr<UBuildingDataBase> BuildingDataBase = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Initialization")
	TObjectPtr<UBuildingSystemSettings> BuildingSystemSettings = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Initialization")
	TObjectPtr<UUnitDataBase> UnitDataBase;
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
