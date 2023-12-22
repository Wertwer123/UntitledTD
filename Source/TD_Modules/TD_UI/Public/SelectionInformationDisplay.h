// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChangeAbleImage.h"
#include "TDStatsComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SelectionInformationDisplay.generated.h"


struct FTDTask;
class UTaskQueueComponent;
class UUniformGridPanel;
class UTextBlock;
class UProgressBar;

UCLASS()
class TD_UI_API USelectionInformationDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	//If something is selected and needs to display a task we do it in here
	
private:
	UPROPERTY(EditAnywhere, Category= "Display")
	int32 MaxTaskRows = 5;

	UPROPERTY(EditAnywhere, Category= "Display")
	int32 MaxTaskColumns = 2;

	UPROPERTY(VisibleAnywhere, Category= "Display")
	int32 CurrentlyDisplayedTasks = 0;
	
	UPROPERTY(EditDefaultsOnly, Category= "Display")
	TSubclassOf<UChangeAbleImage> TaskImageTemplate;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "Display")
	TObjectPtr<UImage> UnitDisplay = nullptr;

	//Need to be dynamically updated
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category= "Display")
	TObjectPtr<UProgressBar> SelectedUnitHealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category= "Display")
	TObjectPtr<UUniformGridPanel> TaskQueue;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category= "Display")
	TObjectPtr<UTextBlock> AttackStat = nullptr;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category= "Display")
	TObjectPtr<UTextBlock> ArmorStat = nullptr;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category= "Display")
	TObjectPtr<UTextBlock> AcidStat = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UnBindFromObjectStatUpdates(const UObject* InSelectedObject);

	UFUNCTION()
	void SetOffensiveStatTextByType(const FTDOffensiveStat& OffensiveStat);
	
	UFUNCTION()
	void SetDefensiveStatByType(const FTDDefensiveStat& OffensiveStat);
	///Based on the currently selected object we set the selection UI
	UFUNCTION()
	void SetSelectionUI(const UObject* InSelectedObject);

	UFUNCTION()
	void AddTaskToDisplay(FTDTask& TaskToAdd);

	UFUNCTION()
	void RemoveTaskFromQueueDisplay();
	
	void FillTaskQueueDisplay(TObjectPtr<UTaskQueueComponent> InTaskQueue);
	void ResetSelectionUIToDefault();
	void ReorderDisplay();
	FText GetTextFromInt(const uint32 IntValue);
};
