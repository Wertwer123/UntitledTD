// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingDataBase.h"
#include "BuildingSystemSettings.h"
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
	TObjectPtr<UBuildingSystemSettings> BuildingSystemSettings = nullptr;
	
	UPROPERTY()
	FOnBuildingModeActivated OnBuildingModeActivated;
	
	UPROPERTY()
	FName CurrentlySelectedBuilding = "NONE";

	void SetCurrentlySelectedBuilding(const FName& SelectedBuilding);
	bool IsBuildingModeActive() const {return bIsBuildingModeActive;}
	
private:

	UPROPERTY()
	TObjectPtr<ABuildingGhost> CurrentlySpawnedBuildingGhost;

	UPROPERTY()
	TObjectPtr<ATDPlayerController> PlayerController;
	
	bool bIsBuildingModeActive = false;
	bool IsSTRGPressed = false;
	
	void SetBuildingMode(const FInputActionValue& InputActionValue);
	void SetSTRGEnabled(const FInputActionValue& InputActionValue);
	void SetSTRGDisabled(const FInputActionValue& InputActionValue);
	void PlaceSelectedBuilding(const FInputActionValue& InputActionValue);
	void SpawnBuildingGhost(const TObjectPtr<UClass> BuildingGhostBP);
	void ClearSelectedBuilding();
	void MoveBuildingGhost();
	bool IsValidBuildingSelected();
	bool IsValidBuildingPlace();
	bool IsBuildingGhostValid();
	bool IsPlaceMultipleBuildingsEnabled();

	///Returns the location and rotation of the hitlocation
	/// We pass in a trace channel to get different results and
	/// informations from the mouse hit
	FTransform GetMouseCursorHitTransform(const ECollisionChannel ChannelToTrace) const;
	
	virtual TStatId GetStatId() const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
};
