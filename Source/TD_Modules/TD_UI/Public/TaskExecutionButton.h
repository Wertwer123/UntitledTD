// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TD_Modules/TDTaskSystem/Public/TDTaskDescription.h"
#include "TaskExecutionButton.generated.h"

class UInputAction;
struct FInputActionValue;

UCLASS()
class TD_UI_API UTaskExecutionButton :  public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category= "Task")
	TObjectPtr<UTaskDescription> TaskToExecute;

	UPROPERTY(EditAnywhere, Category= "Building")
	TObjectPtr<UInputAction> ShortCut = nullptr;
	
	UPROPERTY(EditAnywhere, meta= (BindWidget))
	TObjectPtr<UButton> BuildingButton;
	
	UFUNCTION()
	void ExecuteAssignedTask(const FInputActionValue& InputActionValue);

	void SetBuildingButtonTexture();
	virtual void NativeOnInitialized() override;
	
public:
	void SetBuildingButtonTexture(const TObjectPtr<UTexture2D> NewTexture);
	void SetAssignedTask(const TObjectPtr<UTaskDescription>& NewTaskDescription);
	
#if WITH_EDITOR
	
	virtual void OnWidgetRebuilt() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif
	
};
