// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSubsystem.h"

#include "StaticHelpers.h"
#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/GameplayStatics.h"

#define EMPTY_BUILDING "NONE"

DEFINE_LOG_CATEGORY(LogBuildingSystem)

bool UBuildingSubsystem::IsBuildingGhostValid()
{
	return CurrentlySpawnedBuildingGhost && !CurrentlySelectedBuilding.IsEqual(EMPTY_BUILDING);
}

bool UBuildingSubsystem::IsPlaceMultipleBuildingsEnabled()
{
	return IsSTRGPressed;
}

void UBuildingSubsystem::SetBuildingMode(const FInputActionValue& InputActionValue)
{
	bIsBuildingModeActive = !bIsBuildingModeActive;
	OnBuildingModeActivated.Execute(bIsBuildingModeActive);
}

void UBuildingSubsystem::SetSTRGEnabled(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogBuildingSystem, Log, TEXT("MULTIPLE buildings enabled"))
	IsSTRGPressed = true;
}

void UBuildingSubsystem::SetSTRGDisabled(const FInputActionValue& InputActionValue)
{
	if(IsValidBuildingSelected())
	{
		ClearSelectedBuilding();
	} 

	UE_LOG(LogBuildingSystem, Log, TEXT("MULTIPLE buildings disabled"))
	IsSTRGPressed = false;
}

void UBuildingSubsystem::SetCurrentlySelectedBuilding(const FName& SelectedBuilding)
{
	if(SelectedBuilding.IsEqual(CurrentlySelectedBuilding)) return;

	ClearSelectedBuilding();
	
	CurrentlySelectedBuilding = SelectedBuilding;
	const TObjectPtr<const UBuildingData> BuildingData = BuildingDataBase->GetBuildingDataByName(SelectedBuilding);

	SpawnBuildingGhost(BuildingData->BuildingGhost);
	
}

void UBuildingSubsystem::PlaceSelectedBuilding(const FInputActionValue& InputActionValue)
{
	//If we have selected a valid building the place it
	//TODO Also check in here for resources etc
	if(!IsValidBuildingSelected()) return;
	if(!IsValidBuildingPlace()) return;
	
	const TObjectPtr<const UBuildingData> BuildingData = BuildingDataBase->GetBuildingDataByName(CurrentlySelectedBuilding);
	//Here get the current mouse location and spawn the actual building there
	const FTransform& SpawnTransform = GetMouseCursorHitTransform(BuildingSystemSettings->BuildableTraceChannel);
	
	if(const TObjectPtr<UWorld> World = GetWorld())
	{
		TObjectPtr<ABuildingActor> BuildingActorToPlace = World->SpawnActor<ABuildingActor>(BuildingData->BuildingToPlace, SpawnTransform);
		BuildingActorToPlace->SetupBuilding(BuildingData);
		
		if(IsPlaceMultipleBuildingsEnabled()) return;
		ClearSelectedBuilding();
	}
}

void UBuildingSubsystem::SpawnBuildingGhost(const TObjectPtr<UClass> BuildingGhostBP)
{
	if(TObjectPtr<UWorld> World = GetWorld())
	{
		const FColor BuildingGhostColor = FColor::Green;
		//Here get the current mouse location and spawn the building ghost there
		const FTransform& SpawnTransform = GetMouseCursorHitTransform(BuildingSystemSettings->BuildableTraceChannel);
		
		CurrentlySpawnedBuildingGhost = World->SpawnActor<ABuildingGhost>(BuildingGhostBP, SpawnTransform);
		
		CurrentlySpawnedBuildingGhost->SetGhostColour(BuildingGhostColor);
	}
}

void UBuildingSubsystem::ClearSelectedBuilding()
{
	if(!IsBuildingGhostValid()) return;
	
	CurrentlySpawnedBuildingGhost->Destroy();
	CurrentlySelectedBuilding = EMPTY_BUILDING;
}

void UBuildingSubsystem::MoveBuildingGhost()
{
	const FTransform NewTransform = GetMouseCursorHitTransform(BuildingSystemSettings->BuildableTraceChannel);
	CurrentlySpawnedBuildingGhost->SetActorTransform(NewTransform);

	if(IsValidBuildingPlace())
	{
		CurrentlySpawnedBuildingGhost->SetGhostColour(FColor::Green);
	}
	else
	{
		CurrentlySpawnedBuildingGhost->SetGhostColour(FColor::Red);
	}
}

bool UBuildingSubsystem::IsValidBuildingSelected()
{
	return !CurrentlySelectedBuilding.IsEqual(EMPTY_BUILDING);
}

bool UBuildingSubsystem::IsValidBuildingPlace()
{
	if(!IsBuildingGhostValid()) return false;

	const FTransform& BuildingTransform = CurrentlySpawnedBuildingGhost->GetActorTransform();
	const FVector& BuildingPosition = BuildingTransform.GetLocation();
	const FQuat& BuildingRotation = BuildingTransform.GetRotation();
	
	const FBoxSphereBounds& BuildingBoxBound = CurrentlySpawnedBuildingGhost->GetStaticMeshComponent()->GetStaticMesh()->GetBounds();
	const FVector& BuildingExtents = BuildingBoxBound.BoxExtent;
	
	FCollisionShape Box = FCollisionShape::MakeBox(BuildingExtents);
	FCollisionQueryParams QueryParams;
	//Ignore Ourselves of course
	QueryParams.AddIgnoredActor(CurrentlySpawnedBuildingGhost);

	if(TObjectPtr<UWorld> World = GetWorld())
	{
		DrawDebugBox(World, BuildingPosition, BuildingExtents, FColor::Orange);
		const bool OverlapsInValidPlace = World->OverlapAnyTestByChannel(BuildingPosition, BuildingRotation, BuildingSystemSettings->NonBuildableTraceChannel, Box, QueryParams);
		return !OverlapsInValidPlace;
	}

	return false;
}

FTransform UBuildingSubsystem::GetMouseCursorHitTransform(const ECollisionChannel ChannelToTrace) const
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

bool UBuildingSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if(const TObjectPtr<UWorld> World = Outer->GetWorld())
	{
		return World->IsGameWorld();
	}

	return false;
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
		const TObjectPtr<const UInputAction> MultipleBuildingSupportAction = PlayerInputComponent->FindInputActionByName(EInputMappingContexts::Building, "IA_MultipleBuildings");
		
		PlayerInputComponent->BindAction(EnterBuildingModeAction, ETriggerEvent::Completed, this, &UBuildingSubsystem::SetBuildingMode);
		PlayerInputComponent->BindAction(PlaceBuildingAction, ETriggerEvent::Completed, this, &UBuildingSubsystem::PlaceSelectedBuilding);
		PlayerInputComponent->BindAction(MultipleBuildingSupportAction, ETriggerEvent::Started, this, &UBuildingSubsystem::SetSTRGEnabled);
		PlayerInputComponent->BindAction(MultipleBuildingSupportAction, ETriggerEvent::Completed, this, &UBuildingSubsystem::SetSTRGDisabled);
	}
}

void UBuildingSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!IsBuildingGhostValid()) return;

	MoveBuildingGhost();
}

#undef EMPTY_BUILDING