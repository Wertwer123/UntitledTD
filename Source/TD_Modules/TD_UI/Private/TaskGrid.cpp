// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGrid.h"

#include "TaskExecutionButton.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetTree.h"
#include "Components/UniformGridSlot.h"
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"

void UTaskGrid::FillTaskGrid(const TArray<TObjectPtr<UTaskDescription>>& Tasks)
{
	ResetTaskGridToDefaultTextures();
	ResetTaskButtons();
	
	for (int32 i = 0; i < Tasks.Num(); ++i)
	{
		const TObjectPtr<UTaskDescription> TaskDescription = Tasks[i];
		TaskButtons[i]->SetAssignedTask(TaskDescription);
	}
}

void UTaskGrid::GenerateTaskButtons()
{
	TObjectPtr<UWidgetBlueprintGeneratedClass> WidgetBlueprintGeneratedClass = Cast<UWidgetBlueprintGeneratedClass>(GetClass());

	TObjectPtr<UPackage> Package = WidgetBlueprintGeneratedClass->GetPackage();
	TObjectPtr<UWidgetBlueprint> MainAsset = Cast<UWidgetBlueprint>(Package->FindAssetInPackage());

	TObjectPtr<UUniformGridPanel> AssetTaskLayout = Cast<UUniformGridPanel>(MainAsset->WidgetTree->FindWidget("TaskLayout"));

	AssetTaskLayout->ClearChildren();
	TaskButtons.Empty();
	
	if(!TaskExectuionButtonToSpawn) return;
	
	for (int32 x = 0; x < Rows; ++x)
	{
		for(int32 y = 0; y < Columns; ++y)
		{
			const FString WidgetName = "ExecutionButton" + FString::FromInt(x) + FString::FromInt(y);
			const TObjectPtr<UTaskExecutionButton> ExecutionButton =
				MainAsset->WidgetTree->ConstructWidget<UTaskExecutionButton>(TaskExectuionButtonToSpawn, FName(WidgetName));
			
			TObjectPtr<UUniformGridSlot> GridSlot = AssetTaskLayout->AddChildToUniformGrid(ExecutionButton, x, y);
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
			GridSlot->SetRow(x);
			GridSlot->SetColumn(y);
		}
	}

	AssetTaskLayout->Modify();

	MainAsset->Modify();
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(MainAsset);
}

void UTaskGrid::ResetTaskButtons()
{
	for (const TObjectPtr<UTaskExecutionButton> TaskExecutionButton : TaskButtons)
	{
		TaskExecutionButton->SetAssignedTask(nullptr);
	}
}

void UTaskGrid::ResetTaskGridToDefault()
{
	FillTaskGrid(DefaultTasks);
}

void UTaskGrid::ResetTaskGridToDefaultTextures()
{
	for (const TObjectPtr<UTaskExecutionButton> TaskExecutionButton : TaskButtons)
	{
		TaskExecutionButton->SetBuildingButtonTexture(DefaultButtonTexture);
	}
}

void UTaskGrid::NativeConstruct()
{
	Super::NativeConstruct();

	FillTaskGrid(DefaultTasks);
}

#if WITH_EDITOR

void UTaskGrid::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TaskButtons.Empty();
	
	for (const TObjectPtr<UWidget> Child : TaskLayout->GetAllChildren())
	{
		if(TObjectPtr<UTaskExecutionButton> ExecutionButton = Cast<UTaskExecutionButton>(Child))
		{
			TaskButtons.Add(ExecutionButton);
		}
	}
}

#endif


