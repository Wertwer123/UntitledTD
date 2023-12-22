// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionSubsystem.h"

#include "PlayerHud.h"
#include "Selectable.h"
#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/GameplayStatics.h"

void USelectionSubsystem::TrySelectSelectable(const FInputActionValue& InputActionValue)
{
	FHitResult OutHitResult;
	ISelectable* Selectable = nullptr;
	
	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(SelectableTraceChannel, true, OutHitResult);

	TObjectPtr<AActor> HitActor = OutHitResult.GetActor();

	if(!HitActor)
	{
		//Then we null the currently selected actor and reset the task grid
		//We also broadcast that we have deselected our current object
		OnObjectDeselected.Execute(CurrentlySelectedObject);
		const TObjectPtr<APlayerHud> PlayerHud = Cast<APlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		PlayerHud->ResetTaskGridToDefault();
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
}
