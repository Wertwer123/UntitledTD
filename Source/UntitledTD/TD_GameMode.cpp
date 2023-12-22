// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_GameMode.h"
#include "BuildingSubsystem.h"
#include "PathFindingSubsystem.h"
#include "SelectionSubsystem.h"

void ATD_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if(const TObjectPtr<UBuildingSubsystem> BuildingSubSystem = GetWorld()->GetSubsystem<UBuildingSubsystem>())
	{
		BuildingSubSystem->BuildingDataBase = BuildingDataBase;
		BuildingSubSystem->BuildingSystemSettings = BuildingSystemSettings;
	}
	if(const TObjectPtr<USelectionSubsystem> SelectionSubsystem = GetWorld()->GetSubsystem<USelectionSubsystem>())
	{
		SelectionSubsystem->SelectionSettings = SelectionSettings;
	}
	if(const TObjectPtr<UPathFindingSubsystem> PathFindingSubsystem = GetWorld()->GetSubsystem<UPathFindingSubsystem>())
	{
		PathFindingSubsystem->GridCollection = GridCollection;
	}
}
