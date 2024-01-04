// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Selectable.h"
#include "SelectionSettings.h"
#include "TDPlayerController.h"
#include "Subsystems/WorldSubsystem.h"
#include "SelectionSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnObjectSelected, const UObject*, SelectedObject);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnObjectDeselected, const UObject*, SelectedObject);

UCLASS()
class PLAYER_API USelectionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bExecutedMassSelection = false;

	UPROPERTY()
	FVector2D StartPositionBoxSelection = FVector2D::Zero();

	UPROPERTY()
	FVector2D EndPositionBoxSelection = FVector2D::Zero();
	
	UPROPERTY()
	TObjectPtr<USelectionSettings> SelectionSettings = nullptr;

	UPROPERTY()
	FOnObjectSelected OnObjectSelected;

	UPROPERTY()
	FOnObjectDeselected OnObjectDeselected;
	
	TObjectPtr<UObject> GetCurrentlySelectedObject() const {return CurrentlySelectedObject;}
	
private:
	UPROPERTY()
	TObjectPtr<UObject> CurrentlySelectedObject = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> CurrentlySelectedObjects{};
	
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> SelectableTraceChannel;
	
	UPROPERTY()
	TObjectPtr<ATDPlayerController> PlayerController = nullptr;

	UFUNCTION()
	void StartCreatingSelectionBox(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void FormSelectionBox(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void MassSelectUnits(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void TrySelectSelectable(const FInputActionValue& InputActionValue);
	
	void SetSelectableTraceChannel();
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};
