// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSelectionButton.h"
#include "BuildingSubsystem.h"
#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TD_Modules/Building/Public/BuildingDataBase.h"

DEFINE_LOG_CATEGORY(LogBuildingUI)

void UBuildingSelectionButton::SetBuildingButtonTexture()
{
	if(!BuildingDataBase) return;
	if(!BuildingButton) return;
	
	if(UTexture2D* BuildingSprite = BuildingDataBase->GetSpriteOfBuilding(BuildingName))
	{
		FButtonStyle NewButtonStyle;
		NewButtonStyle.Normal.SetResourceObject(BuildingSprite);
		
		BuildingButton->SetStyle(NewButtonStyle);
	}
}

TArray<FName> UBuildingSelectionButton::GetAllBuildings() const
{
	if(!BuildingDataBase) return {FName("NONE")};
	
	return BuildingDataBase->GetAllBuildingNames();
}

void UBuildingSelectionButton::SelectBuilding(const FInputActionValue& InputActionValue)
{
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	
	if(!BuildingSubsystem->IsBuildingModeActive()) return;
	
	BuildingSubsystem->SetCurrentlySelectedBuilding(BuildingName);
}

void UBuildingSelectionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TObjectPtr<ATDPlayerController> PlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if(PlayerController)
	{
		PlayerController->GetPlayerInputComponent()->BindAction(ShortCut, ETriggerEvent::Completed, this, &UBuildingSelectionButton::SelectBuilding);
	}
}

#if WITH_EDITOR

void UBuildingSelectionButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	SetBuildingButtonTexture();
}



void UBuildingSelectionButton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetBuildingButtonTexture();
}

#endif
