// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingDataBase.h"
#include "InputActionValue.h"
#include "Subsystems/WorldSubsystem.h"
#include "BuildingSubsystem.generated.h"

class ATDPlayerController;

DECLARE_LOG_CATEGORY_EXTERN(LogBuildingSystem, Log, All)

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBuildingModeActivated, bool, BuildingModeIsActive);

UCLASS()
class BUILDING_API UBuildingSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TObjectPtr<UBuildingDataBase> BuildingDataBase = nullptr;
	
	UPROPERTY()
	FOnBuildingModeActivated OnBuildingModeActivated;

	UPROPERTY()
	FName CurrentlySelectedBuilding = "NONE";

	void SetCurrentlySelectedBuilding(const FName& SelectedBuilding);
	bool IsBuildingModeActive() const {return bIsBuildingModeActive;}
	
private:

	UPROPERTY()
	TObjectPtr<AActor> CurrentlySpawnedBuildingGhost;

	UPROPERTY()
	TObjectPtr<ATDPlayerController> PlayerController;
	
	bool bIsBuildingModeActive = false;

	bool IsBuildingGhostValid();
	void SetBuildingMode(const FInputActionValue& InputActionValue);
	void PlaceSelectedBuilding(const FInputActionValue& InputActionValue);
	void SpawnBuildingGhost(const TObjectPtr<UClass> BuildingGhostBP);
	void MoveBuildingGhost();

	///Returns the location and rotation of the hitlocation
	FTransform GetMouseCursorHitTransform() const;
	
	virtual TStatId GetStatId() const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
};
