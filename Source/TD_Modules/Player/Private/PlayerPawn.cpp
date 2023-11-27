// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
	//Set to false now as the movement gets handled by the player controller maybe we manage in here the building inputs
	PrimaryActorTick.bCanEverTick = false;
	Camera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
}

void APlayerPawn::SetInitialPlayerRotation()
{
	FRotator InitialRoatation(InitialYRotation, 0, 0);
	SetActorRotation(InitialRoatation);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetInitialPlayerRotation();
}

