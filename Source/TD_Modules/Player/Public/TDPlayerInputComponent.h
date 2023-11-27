// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TDPlayerInputComponent.generated.h"

class UInputAction;

UENUM()
enum class EInputMappingContexts
{
	Default,
	Building
};

UCLASS
()
class PLAYER_API UTDPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	void SetUpPlayerInput(APlayerController* PlayerController);
	TObjectPtr<UInputMappingContext> GetCurrentPlayerMappingContext() {return DefaultMappingContext;}
	TObjectPtr<const UInputAction> FindInputActionByName(const EInputMappingContexts ContextToGet ,const FName& InputActionToFind);
	
private:
	UPROPERTY(EditAnywhere, Category= InputContext)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category= InputContext)
	TObjectPtr<UInputMappingContext> BuildingModeMapping;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> CurrentlyHighestPriorityMappingContext = nullptr;

	UFUNCTION()
	void OnBuildingModeSet(bool bIsBuildingModeActive);

	void SetUpMappingContext(APlayerController* PlayerController);
	TObjectPtr<UInputMappingContext> GetMappingContextByIdentifier(const EInputMappingContexts ContextToGet);
};
