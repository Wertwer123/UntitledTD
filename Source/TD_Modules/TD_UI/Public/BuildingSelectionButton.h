// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingDataBase.h"
#include "InputAction.h"
#include "Components/Button.h"
#include "BuildingSelectionButton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBuildingUI, Log, All);

struct FInputActionValue;

UCLASS()
class TD_UI_API UBuildingSelectionButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetBuildingButtonTexture();

private:
	
	UPROPERTY(EditAnywhere, meta= (GetOptions= "GetAllBuildings"), Category= "Building")
	FName BuildingName = FName();

	UPROPERTY(EditAnywhere, Category= "Building")
	TObjectPtr<UInputAction> ShortCut = nullptr;
	
	UPROPERTY(EditAnywhere, meta= (BindWidget))
	UButton*  BuildingButton;
	
	UPROPERTY(EditAnywhere, Category= Building)
	UBuildingDataBase* BuildingDataBase;

	UFUNCTION()
	TArray<FName> GetAllBuildings() const;
	
	UFUNCTION()
	void SelectBuilding(const FInputActionValue& InputActionValue);
	
	virtual void NativeOnInitialized() override;
	
#if WITH_EDITOR
	
	virtual void OnWidgetRebuilt() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif
};
