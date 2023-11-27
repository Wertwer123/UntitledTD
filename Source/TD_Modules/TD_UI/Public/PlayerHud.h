#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Components/Widget.h"
#include "GameFramework/HUD.h"
#include "PlayerHud.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerHud,Log, All)

UCLASS()
class TD_UI_API APlayerHud : public AHUD
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category= "HUD")
	TArray<TSubclassOf<UUserWidget>> WidgetsToSpawnWithoutFurtherConfig;

	UPROPERTY()
	TArray<TObjectPtr<UWidget>> SpawnedWidgets;
	
	virtual void BeginPlay() override;
	void InitializeHud();
};
