// Fill out your copyright notice in the Description page of Project Settings.

#include "TDPlayerInputComponent.h"
#include "BuildingSubsystem.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet/GameplayStatics.h"

void UTDPlayerInputComponent::SetUpPlayerInput(APlayerController* PlayerController)
{
	if(!PlayerController) return;

	//Bind to the building mode set delegate so that we add the building mode mapping context
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	BuildingSubsystem->OnBuildingModeActivated.BindDynamic(this, &UTDPlayerInputComponent::OnBuildingModeSet);
	
	SetUpMappingContext(PlayerController);
}

TObjectPtr<const UInputAction> UTDPlayerInputComponent::FindInputActionByName(const EInputMappingContexts ContextToGet ,const FName& InputActionToFind)
{
	TObjectPtr<const UInputAction> FoundInputAction = nullptr;
	const TObjectPtr<UInputMappingContext> MappingToFindAction = GetMappingContextByIdentifier(ContextToGet);
	
	for (auto InputAction : MappingToFindAction->GetMappings())
	{
		//If we ever get an error in her
		if(InputAction.Action.GetFName().IsEqual(InputActionToFind))
		{
			FoundInputAction = InputAction.Action;
			break;
		}
	}

	ensureMsgf(FoundInputAction, TEXT("WE didnt find the action did you rename or delete it ? "));

	return  FoundInputAction;
}

void UTDPlayerInputComponent::OnBuildingModeSet(bool bIsBuildingModeActive)
{
	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
	if(UEnhancedInputLocalPlayerSubsystem* PlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if(bIsBuildingModeActive)
		{
			CurrentlyHighestPriorityMappingContext = BuildingModeMapping;
			PlayerInputSubsystem->AddMappingContext(BuildingModeMapping, 1);
			UE_LOG(LogTemp, Log, TEXT("We set the building mode active"))
		}
		else
		{
			CurrentlyHighestPriorityMappingContext = DefaultMappingContext;
			PlayerInputSubsystem->RemoveMappingContext(BuildingModeMapping);
			UE_LOG(LogTemp, Log, TEXT("We set the building mode inactive"))
		}
	}
}

TObjectPtr<UInputMappingContext> UTDPlayerInputComponent::GetMappingContextByIdentifier(const EInputMappingContexts ContextToGet)
{
	switch (ContextToGet)
	{
	case EInputMappingContexts::Default:
		return DefaultMappingContext;
		
	case EInputMappingContexts::Building:
		return BuildingModeMapping;
		
	default: return nullptr;
	}
}

void UTDPlayerInputComponent::SetUpMappingContext(APlayerController* PlayerController)
{
	if(UEnhancedInputLocalPlayerSubsystem* PlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		CurrentlyHighestPriorityMappingContext = DefaultMappingContext;
		
		PlayerInputSubsystem->ClearAllMappings();
		PlayerInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
