#include "PlayerHud.h"
#include "BuildingActor.h"
#include "BuildingDataBase.h"
#include "TaskGrid.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogPlayerHud)

void APlayerHud::ExchangeTaskGrid(const TArray<TObjectPtr<UTaskDescription>> NewTasks)
{
	if(!TaskGrid)
	{
		UE_LOG(LogPlayerHud, Error, TEXT("No Task grid was initialized"))
		return;
	}

	TaskGrid->FillTaskGrid(NewTasks);
}

void APlayerHud::ResetTaskGridToDefault() const
{
	TaskGrid->ResetTaskGridToDefault();
}

void APlayerHud::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeHud();
}

void APlayerHud::InitializeHud()
{
	InitializeHudUI();
	
	for (const auto WidgetClass : WidgetsToSpawnWithoutFurtherConfig)
	{
		TObjectPtr<UUserWidget> CreatedWidget = CreateWidget(GetOwningPlayerController(),WidgetClass);
		SpawnedWidgets.Add(CreatedWidget);
		Root->CanvasPanel->AddChildToCanvas(CreatedWidget);
		CreatedWidget->AddToViewport();
	}

	AUnit::OnUnitSelected.BindDynamic(this, &APlayerHud::OnUnitSelected);
	ABuildingActor::OnBuildingSelected.BindDynamic(this, &APlayerHud::OnBuildingSelected);
}

void APlayerHud::InitializeHudUI()
{
	Root = CreateWidget<UPlayerHudUI>(GetOwningPlayerController(), RootWidgetClass);
	Root->AddToViewport();

	TaskGrid = Root->TaskGrid;

	SpawnedWidgets.Add(Root);
}

void APlayerHud::OnBuildingSelected(const FName& BuildingName)
{
	if(!BuildingDataBase) return;

	TObjectPtr<const UBuildingData> BuildingData = BuildingDataBase->GetBuildingDataByName(BuildingName);

	//TEMP
	if(!BuildingData) return;
	
	ExchangeTaskGrid(BuildingData->TaskThisBuildingIsAbleToExecute);
}

void APlayerHud::OnUnitSelected(const FName& UnitName)
{
	if(!UnitDataBase) return;

	TObjectPtr<const UUnitData> UnitData = UnitDataBase->GetUnit(UnitName);

	//TEMP
	if(!UnitData) return;
	
	ExchangeTaskGrid(UnitData->GetTasksThisUnitIsAbleToExecute());
}

