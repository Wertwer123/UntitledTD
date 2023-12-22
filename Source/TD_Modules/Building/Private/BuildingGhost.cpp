// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGhost.h"


// Sets default values
ABuildingGhost::ABuildingGhost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetCollisionProfileName("Building");
}

void ABuildingGhost::SetGhostColour(const FColor& NewColor)
{
	if(IsColorAlreadySet(NewColor)) return;
	
	TObjectPtr<UStaticMeshComponent> SMC = GetStaticMeshComponent();
	
	const uint8 R = NewColor.R;
	const uint8 G = NewColor.G;
	const uint8 B = NewColor.B;
	constexpr uint8 A = 1;

	const FVector4 Color(R, G, B, A);
	SMC->SetCustomPrimitiveDataVector4(0, Color);
}

bool ABuildingGhost::IsColorAlreadySet(const FColor& ColorToCheck)
{
	TObjectPtr<UStaticMeshComponent> SMC = GetStaticMeshComponent();
	
	const uint8 CurrentR = SMC->GetCustomPrimitiveData().Data[0];
	const uint8 CurrentG = SMC->GetCustomPrimitiveData().Data[1];
	const uint8 CurrentB = SMC->GetCustomPrimitiveData().Data[2];
	const uint8 CurrentA = SMC->GetCustomPrimitiveData().Data[3];

	return ColorToCheck.R == CurrentR && ColorToCheck.G == CurrentG
	&& ColorToCheck.B == CurrentB;
}
