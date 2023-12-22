#pragma once

#include "CoreMinimal.h"
#include "PlayerHudUI.h"
#include "UnitDataBase.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Components/Widget.h"
#include "GameFramework/HUD.h"
#include "PlayerHud.generated.h"

class UBuildingDataBase;
class UTaskDescription;
class UTaskGrid;
DECLARE_LOG_CATEGORY_EXTERN(LogPlayerHud, Log, All)

UCLASS()
class TD_UI_API APlayerHud : public AHUD
{
	GENERATED_BODY()

public:
	///Needs to be copied because we cant assign a reference of an TArray to an reference
	///to an reference to TArray
	void ExchangeTaskGrid(const TArray<TObjectPtr<UTaskDescription>> NewTasks);
	void ResetTaskGridToDefault() const;
	TObjectPtr<UPlayerHudUI> GetRoot() const {return Root;};
private:

	UPROPERTY(EditAnywhere, Category= "HUD")
	TObjectPtr<UBuildingDataBase> BuildingDataBase;

	UPROPERTY(EditAnywhere, Category= "HUD")
	TObjectPtr<UUnitDataBase> UnitDataBase;
	
	UPROPERTY(VisibleAnywhere, Category= "HUD")
	TObjectPtr<UPlayerHudUI> Root = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category= "HUD")
	TObjectPtr<UTaskGrid> TaskGrid = nullptr;
	
	UPROPERTY(EditAnywhere, Category= "HUD")
	TArray<TSubclassOf<UUserWidget>> WidgetsToSpawnWithoutFurtherConfig;

	UPROPERTY(EditAnywhere, Category= "HUD")
	TSubclassOf<UPlayerHudUI> RootWidgetClass;
	
	UPROPERTY()
	TArray<TObjectPtr<UWidget>> SpawnedWidgets;
	
	UFUNCTION()
	void OnBuildingSelected(const FName& BuildingName);

	UFUNCTION()
	void OnUnitSelected(const FName& UnitName);

	void InitializeHud();
	void InitializeHudUI();
	virtual void BeginPlay() override;
};
