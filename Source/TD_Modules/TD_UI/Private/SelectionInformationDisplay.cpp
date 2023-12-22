// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionInformationDisplay.h"
#include "BuildingActor.h"
#include "ChangeAbleImage.h"
#include "SelectionSubsystem.h"
#include "TaskQueueComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void USelectionInformationDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	ResetSelectionUIToDefault();
	
	TObjectPtr<USelectionSubsystem> SelectionSubsystem = GetWorld()->GetSubsystem<USelectionSubsystem>();
	SelectionSubsystem->OnObjectSelected.BindDynamic(this, &USelectionInformationDisplay::SetSelectionUI);
	SelectionSubsystem->OnObjectDeselected.BindDynamic(this, &USelectionInformationDisplay::UnBindFromObjectStatUpdates);
}

void USelectionInformationDisplay::UnBindFromObjectStatUpdates(const UObject* DeselectedObject)
{
	//No Object was selected so we dont need to unbind
	if(!DeselectedObject) return;
	
	if(TObjectPtr<const AActor> DeselectedActor = Cast<AActor>(DeselectedObject))
	{
		//Unbind from stats events
		const TObjectPtr<UTDStatsComponent> Stats = Cast<UTDStatsComponent>(DeselectedActor->GetComponentByClass(UTDStatsComponent::StaticClass()));
		
		Stats->OnDefensiveStatUpdated.RemoveDynamic(this, &USelectionInformationDisplay::SetDefensiveStatByType);
		Stats->OnOffensiveStatUpdated.RemoveDynamic(this, &USelectionInformationDisplay::SetOffensiveStatTextByType);

		//Unbind form the task queue events
		const TObjectPtr<UTaskQueueComponent> TaskQueueComponent = Cast<UTaskQueueComponent>(DeselectedActor->GetComponentByClass(UTaskQueueComponent::StaticClass()));
		TaskQueueComponent->OnTaskAdded.RemoveDynamic(this, &USelectionInformationDisplay::AddTaskToDisplay);

		for (const TSharedPtr<FTDTask> Task : TaskQueueComponent->GetCurrentTasks())
		{
			Task->OnTaskCompleted.RemoveDynamic(this, &USelectionInformationDisplay::RemoveTaskFromQueueDisplay);
		}
	}

	//And reset the Ui to default
	ResetSelectionUIToDefault();
}

void USelectionInformationDisplay::SetOffensiveStatTextByType(const FTDOffensiveStat& OffensiveStat)
{
	switch (OffensiveStat.GetStatType())
	{
	case ETDOffensiveStatType::Empty:
		{
			//Nothing should happen
			break;
		}
	case ETDOffensiveStatType::AttackDmg:
		{
			AttackStat->SetText(GetTextFromInt(OffensiveStat.GetCurrentStatValue()));
			break;
		}
	case ETDOffensiveStatType::Range:
		{
			break;
		}
	case ETDOffensiveStatType::ArmorPierce:
		{
			
			break;
		}
	case ETDOffensiveStatType::Health:
		{
			SelectedUnitHealthBar->SetPercent(static_cast<float>(OffensiveStat.GetCurrentStatValue() / OffensiveStat.GetMaxStatValue()));
			break;
		}
	default: ;
	}
}

void USelectionInformationDisplay::SetDefensiveStatByType(const FTDDefensiveStat& DefensiveStat)
{
	switch (DefensiveStat.GetStatType())
	{
	case ETDDefensiveStatType::Empty:
		{
			//Nothing should happen initialize with none
			break;
		}
	case ETDDefensiveStatType::Armor:
		{
			ArmorStat->SetText(GetTextFromInt(DefensiveStat.GetCurrentResistance()));
			break;
		}
	case ETDDefensiveStatType::AcidResistance:
		{
			AcidStat->SetText(GetTextFromInt(DefensiveStat.GetCurrentResistance()));
			break;
		}
	case ETDDefensiveStatType::PiercingResistance:
		{
			break;
		}
	default: ;
	}
}

void USelectionInformationDisplay::SetSelectionUI(const UObject* InSelectedObject)
{
	ResetSelectionUIToDefault();
	
	if(TObjectPtr<const AActor> SelectedActor = Cast<AActor>(InSelectedObject))
	{
		//Activate the healthbar
		SelectedUnitHealthBar->SetVisibility(ESlateVisibility::Visible);
		
		TObjectPtr<UTDStatsComponent> ActorStats = Cast<UTDStatsComponent>(SelectedActor->GetComponentByClass(UTDStatsComponent::StaticClass()));
		TObjectPtr<UTaskQueueComponent> ActorTaskQueue = Cast<UTaskQueueComponent>(SelectedActor->GetComponentByClass(UTaskQueueComponent::StaticClass()));

		ActorStats->OnDefensiveStatUpdated.AddDynamic(this, &USelectionInformationDisplay::SetDefensiveStatByType);
		ActorStats->OnOffensiveStatUpdated.AddDynamic(this, &USelectionInformationDisplay::SetOffensiveStatTextByType);

		FillTaskQueueDisplay(ActorTaskQueue);
		
		for (FTDOffensiveStat& OffensiveStat : ActorStats->GetAllOffensiveStats())
		{
			SetOffensiveStatTextByType(OffensiveStat);
		}
		
		for (FTDDefensiveStat& DefensiveStat : ActorStats->GetAllDefensiveStats())
		{
			SetDefensiveStatByType(DefensiveStat);
		}
	}
}

void USelectionInformationDisplay::AddTaskToDisplay(FTDTask& TaskToAdd)
{
	TObjectPtr<UChangeAbleImage> TaskDisplay = CreateWidget<UChangeAbleImage>(this, TaskImageTemplate);
	TaskDisplay->SetImage(TaskToAdd.GetTaskDisplay());

	TaskToAdd.OnTaskCompleted.AddDynamic(this, &USelectionInformationDisplay::RemoveTaskFromQueueDisplay);
	//Add pluss one to the displayed tasks and add it at this position
	
	UUniformGridSlot* GridSlot = TaskQueue->AddChildToUniformGrid(TaskDisplay, 0, CurrentlyDisplayedTasks);
	
	CurrentlyDisplayedTasks++;
}

void USelectionInformationDisplay::RemoveTaskFromQueueDisplay()
{
	TaskQueue->RemoveChildAt(0);
	ReorderDisplay();
	
	CurrentlyDisplayedTasks--;
}

void USelectionInformationDisplay::FillTaskQueueDisplay(TObjectPtr<UTaskQueueComponent> InTaskQueue)
{
	InTaskQueue->OnTaskAdded.AddDynamic(this, &USelectionInformationDisplay::AddTaskToDisplay);
	
	for (int32 i = 0; i < InTaskQueue->GetCurrentTasks().Num(); ++i)
	{
		const TSharedPtr<FTDTask>& Task = InTaskQueue->GetCurrentTasks()[i];
		Task->OnTaskCompleted.AddDynamic(this, &USelectionInformationDisplay::RemoveTaskFromQueueDisplay);

		TObjectPtr<UChangeAbleImage> TaskDisplay = CreateWidget<UChangeAbleImage>(this, TaskImageTemplate);
		TaskDisplay->SetImage(Task->GetTaskDisplay());
		
		UUniformGridSlot* GridSlot = TaskQueue->AddChildToUniformGrid(TaskDisplay, 0, CurrentlyDisplayedTasks);
		GridSlot->SetColumn(i);
		TaskDisplay->AddToViewport();

		CurrentlyDisplayedTasks++;
		
	}
}

void USelectionInformationDisplay::ResetSelectionUIToDefault()
{
	//Clear all text
	AttackStat->SetText(FText::GetEmpty());
	ArmorStat->SetText(FText::GetEmpty());
	AcidStat->SetText(FText::GetEmpty());

	TaskQueue->ClearChildren();
	SelectedUnitHealthBar->SetVisibility(ESlateVisibility::Hidden);
	CurrentlyDisplayedTasks = 0;
}

void USelectionInformationDisplay::ReorderDisplay()
{
	for (int32 i = 0; i < TaskQueue->GetAllChildren().Num(); ++i)
	{
		const UWidget* ChildWidget = TaskQueue->GetAllChildren()[i];
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ChildWidget->Slot);
		GridSlot->SetColumn(i);
	}
}

FText USelectionInformationDisplay::GetTextFromInt(const uint32 IntValue)
{
	return FText::FromString(FString::FromInt(IntValue));
}
