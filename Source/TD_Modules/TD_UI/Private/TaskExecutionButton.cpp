#include "TaskExecutionButton.h"
#include "TaskReciever.h"
#include "SelectionSubsystem.h"
#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/GameplayStatics.h"

void UTaskExecutionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TObjectPtr<ATDPlayerController> PlayerController = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if(PlayerController)
	{
		PlayerController->GetPlayerInputComponent()->BindAction(ShortCut, ETriggerEvent::Completed, this, &UTaskExecutionButton::ExecuteAssignedTask);
	}
}

void UTaskExecutionButton::SetBuildingButtonTexture(const TObjectPtr<UTexture2D> NewTexture)
{
	FButtonStyle NewButtonStyle;
	NewButtonStyle.Normal.SetResourceObject(NewTexture);
		
	BuildingButton->SetStyle(NewButtonStyle);
}

void UTaskExecutionButton::SetAssignedTask(const TObjectPtr<UTaskDescription>& NewTaskDescription)
{
	TaskToExecute = NewTaskDescription;
	
	SetBuildingButtonTexture();
}


void UTaskExecutionButton::ExecuteAssignedTask(const FInputActionValue& InputActionValue)
{
	if(!TaskToExecute) return;

	TObjectPtr<USelectionSubsystem> SelectionSubsystem = GetWorld()->GetSubsystem<USelectionSubsystem>();
	const TObjectPtr<UObject> CurrentlySelectedObject = SelectionSubsystem->GetCurrentlySelectedObject();
	
	if(CurrentlySelectedObject)
	{
		//If we have a currently selected object than we pass this one into the action because the assigned action
		//Should be the actions of the object

		//Cast for a task reciever meaning the selected thing is able to recieve tasks
		if(ITaskReciever* TaskReciever = Cast<ITaskReciever>(CurrentlySelectedObject))
		{
			TaskReciever->RecieveTask(TaskToExecute->CreateTaskObject(CurrentlySelectedObject));
		}
	}
	else
	{
		//If we dont have a object selected we assume that we have the default actions selected
		TaskToExecute->CreateTaskObject(this)->ExecuteTask();
	}
}

void UTaskExecutionButton::SetBuildingButtonTexture()
{
	if(!TaskToExecute) return;
	
	if(UTexture2D* BuildingSprite = TaskToExecute->GetTaskDisplay())
	{
		FButtonStyle NewButtonStyle;
		NewButtonStyle.Normal.SetResourceObject(BuildingSprite);
		
		BuildingButton->SetStyle(NewButtonStyle);
	}
}

#if WITH_EDITOR

void UTaskExecutionButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	SetBuildingButtonTexture();
}



void UTaskExecutionButton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetBuildingButtonTexture();
}

#endif