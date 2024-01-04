// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionSubsystem.h"

#include "PlayerHud.h"
#include "Selectable.h"
#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/GameplayStatics.h"

void USelectionSubsystem::StartCreatingSelectionBox(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Started Creating SelectionBox"))
	FHitResult OutHitResult;
	
	float LocationX;
	float LocationY;
	UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(LocationX, LocationY);
	StartPositionBoxSelection = FVector2D(LocationX, LocationY);
	EndPositionBoxSelection = FVector2D(LocationX, LocationY);

	UE_LOG(LogTemp, Warning, TEXT("Start position screen for box selection %s "), *StartPositionBoxSelection.ToString())
	UE_LOG(LogTemp, Warning, TEXT("End position screen  for box selection %s "), *EndPositionBoxSelection.ToString())
}

void USelectionSubsystem::FormSelectionBox(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Froming selection Box"))
	float LocationX;
	float LocationY;
	UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(LocationX, LocationY);

	EndPositionBoxSelection = FVector2D(LocationX, LocationY);

	UE_LOG(LogTemp, Warning, TEXT("End position screen  for box selection %s "), *EndPositionBoxSelection.ToString())
}

void USelectionSubsystem::MassSelectUnits(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Wasnt able to execute single select because we were mass selecting"))
}

void USelectionSubsystem::TrySelectSelectable(const FInputActionValue& InputActionValue)
{
	if(bExecutedMassSelection)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wasnt able to execute single select because we were mass selecting"))
		return;
	}
	
	FHitResult OutHitResult;
	ISelectable* Selectable = nullptr;
	
	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(SelectableTraceChannel, true, OutHitResult);

	TObjectPtr<AActor> HitActor = OutHitResult.GetActor();

	if(!HitActor && CurrentlySelectedObject)
	{
		//Then we null the currently selected actor and reset the task grid
		//We also broadcast that we have deselected our current object
		OnObjectDeselected.Execute(CurrentlySelectedObject);
		const TObjectPtr<APlayerHud> PlayerHud = Cast<APlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		PlayerHud->ResetTaskGridToDefault();
		Cast<ISelectable>(CurrentlySelectedObject)->OnDeselect();
		CurrentlySelectedObject = nullptr;
		return;
	}
	
	Selectable = Cast<ISelectable>(HitActor);
	
	if(Selectable && HitActor != CurrentlySelectedObject)
	{
		OnObjectDeselected.Execute(CurrentlySelectedObject);
		CurrentlySelectedObject = HitActor;
		OnObjectSelected.Execute(CurrentlySelectedObject);
		Selectable->OnSelect();
	}
}

void USelectionSubsystem::SetSelectableTraceChannel()
{
	SelectableTraceChannel = SelectionSettings->SelectableCollisionChannel; 
}

void USelectionSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	SetSelectableTraceChannel();

	PlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if(!PlayerController) return;

	const TObjectPtr<UTDPlayerInputComponent> PlayerInputComponent = PlayerController->GetPlayerInputComponent();
	const TObjectPtr<const UInputAction> SelectBuildingInputAction = PlayerInputComponent->FindInputActionByName(EInputMappingContexts::Default, "IA_SelectBuilding");
	
	PlayerInputComponent->BindAction(SelectBuildingInputAction, ETriggerEvent::Completed, this, &USelectionSubsystem::TrySelectSelectable);
	PlayerInputComponent->BindAction(SelectBuildingInputAction, ETriggerEvent::Started, this, &USelectionSubsystem::StartCreatingSelectionBox);
	PlayerInputComponent->BindAction(SelectBuildingInputAction, ETriggerEvent::Triggered, this, &USelectionSubsystem::FormSelectionBox);
	PlayerInputComponent->BindAction(SelectBuildingInputAction, ETriggerEvent::Completed, this, &USelectionSubsystem::MassSelectUnits);
}
