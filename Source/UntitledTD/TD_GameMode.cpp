// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_GameMode.h"

#include "BuildingSubsystem.h"

void ATD_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if(TObjectPtr<UBuildingSubsystem> BuildingSubSystem = GetWorld()->GetSubsystem<UBuildingSubsystem>())
	{
		UE_LOG(LogTemp, Log, TEXT("We set the building data base in the building subsystem"))
		BuildingSubSystem->BuildingDataBase = BuildingDataBase;
	}
}
