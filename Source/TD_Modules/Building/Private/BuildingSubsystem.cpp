// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSubsystem.h"

#include "StaticHelpers.h"
#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogBuildingSystem)

bool UBuildingSubsystem::IsBuildingGhostValid()
{
	return CurrentlySpawnedBuildingGhost && !CurrentlySelectedBuilding.IsEqual("NONE");
}

void UBuildingSubsystem::SetBuildingMode(const FInputActionValue& InputActionValue)
{
	bIsBuildingModeActive = !bIsBuildingModeActive;
	OnBuildingModeActivated.Execute(bIsBuildingModeActive);
}

void UBuildingSubsystem::SetCurrentlySelectedBuilding(const FName& SelectedBuilding)
{
	if(SelectedBuilding.IsEqual(CurrentlySelectedBuilding)) return;
	
	CurrentlySelectedBuilding = SelectedBuilding;
	const TObjectPtr<const UBuildingData> BuildingData = BuildingDataBase->GetBuildingDataByName(SelectedBuilding);

	SpawnBuildingGhost(BuildingData->BuildingGhost);
	
}

void UBuildingSubsystem::PlaceSelectedBuilding(const FInputActionValue& InputActionValue)
{
	const TObjectPtr<const UBuildingData> BuildingToPlace = BuildingDataBase->GetBuildingDataByName(CurrentlySelectedBuilding);
	//Here get the current mouse location and spawn the actual building there
	const FTransform& SpawnTransform = GetMouseCursorHitTransform();
	
	if(TObjectPtr<UWorld> World = GetWorld())
	{
		World->SpawnActor(BuildingToPlace->BuildingToPlace, &SpawnTransform);
	}
}

void UBuildingSubsystem::SpawnBuildingGhost(const TObjectPtr<UClass> BuildingGhostBP)
{
	if(TObjectPtr<UWorld> World = GetWorld())
	{
		//Here get the current mouse location and spawn the building ghost there
		const FTransform& SpawnTransform = GetMouseCursorHitTransform();
		
		CurrentlySpawnedBuildingGhost = World->SpawnActor(BuildingGhostBP, &SpawnTransform);
	}
}

void UBuildingSubsystem::MoveBuildingGhost()
{
	const FTransform NewTransform = GetMouseCursorHitTransform();
	CurrentlySpawnedBuildingGhost->SetActorTransform(NewTransform);
}

FTransform UBuildingSubsystem::GetMouseCursorHitTransform() const
{
	FHitResult GroundHitLocation;
	PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, true, GroundHitLocation);

	const FVector& NormalOfGround = GroundHitLocation.Normal;

	const double AngleBetweenUpVectorAndNormal = FStaticHelpers::GetAngleBetweenVectors(FVector::UpVector, NormalOfGround);
	
	const FRotator RotationToApplyOnXToOrientUpVectorToNormal(-90, 0, 0);
	const FRotator BuildingRotation = NormalOfGround.Rotation() + RotationToApplyOnXToOrientUpVectorToNormal;
	
	FTransform HitTransform;
	HitTransform.SetLocation(GroundHitLocation.Location);
	HitTransform.SetRotation(BuildingRotation.Quaternion());
	
	return HitTransform;
}

TStatId UBuildingSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildingSubsystem, STATGROUP_Tickables);
}

void UBuildingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	bIsBuildingModeActive = false;
}

void UBuildingSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	bIsBuildingModeActive = false;
}

void UBuildingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	PlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if(PlayerController)
	{
		const TObjectPtr<UTDPlayerInputComponent> PlayerInputComponent = PlayerController->GetPlayerInputComponent();
		const TObjectPtr<const UInputAction> EnterBuildingModeAction = PlayerInputComponent->FindInputActionByName(EInputMappingContexts::Default, "IA_EnterBuildingMode");
		const TObjectPtr<const UInputAction> PlaceBuildingAction = PlayerInputComponent->FindInputActionByName(EInputMappingContexts::Building, "IA_PlaceBuilding");
		
		PlayerInputComponent->BindAction(EnterBuildingModeAction, ETriggerEvent::Completed, this, &UBuildingSubsystem::SetBuildingMode);
		PlayerInputComponent->BindAction(PlaceBuildingAction, ETriggerEvent::Completed, this, &UBuildingSubsystem::PlaceSelectedBuilding);
	}
}

void UBuildingSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!IsBuildingGhostValid()) return;

	MoveBuildingGhost();
}
