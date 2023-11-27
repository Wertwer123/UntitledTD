// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHud.h"

#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogPlayerHud)

void APlayerHud::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogPlayerHud, Warning, TEXT("The hud was created"))

	InitializeHud();
}

void APlayerHud::InitializeHud()
{
	for (const auto WidgetClass : WidgetsToSpawnWithoutFurtherConfig)
	{
		TObjectPtr<UUserWidget> CreatedWidget = CreateWidget(GetOwningPlayerController(),WidgetClass.Get());
		SpawnedWidgets.Add(CreatedWidget);
		CreatedWidget->AddToViewport();
	}
}
