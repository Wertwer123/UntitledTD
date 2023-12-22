// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "TaskGrid.generated.h"

class UTaskDescription;
class UTaskExecutionButton;

UCLASS()
class TD_UI_API UTaskGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void FillTaskGrid(const TArray<TObjectPtr<UTaskDescription>>& Tasks);
	void ResetTaskGridToDefault();
	
private:
	UPROPERTY(EditAnywhere, Category= "Layout")
	int32 Columns = 3;

	UPROPERTY(EditAnywhere, Category= "Layout")
	int32 Rows = 3;

	UPROPERTY(EditAnywhere, Category= "Setup")
	TArray<TObjectPtr<UTaskDescription>> DefaultTasks;
	
	UPROPERTY(EditAnywhere, Category= "TaskButtons")
	TObjectPtr<UTexture2D> DefaultButtonTexture;
	
	UPROPERTY(EditAnywhere, Category= "TaskButtons")
	TSubclassOf<UTaskExecutionButton> TaskExectuionButtonToSpawn;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category= "Layout")
	TObjectPtr<UUniformGridPanel> TaskLayout;

	UPROPERTY(EditAnywhere, Category= "Layout")
	TArray<TObjectPtr<UTaskExecutionButton>> TaskButtons;

	UFUNCTION(CallInEditor, Category= "Setup")
	void GenerateTaskButtons();

	void ResetTaskButtons();
	void ResetTaskGridToDefaultTextures();
	virtual void NativeConstruct() override;
	
#if WITH_EDITOR

	virtual void SynchronizeProperties() override;
	
#endif
};
