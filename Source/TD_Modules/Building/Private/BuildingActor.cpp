#include "BuildingActor.h"
#include "BuildingData.h"
#include "PlayerHud.h"
#include "TaskQueueComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogBuildings)

ABuildingActor::ABuildingActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StatsComponent = CreateDefaultSubobject<UTDStatsComponent>("BuildingStats");
	TaskQueueComponent = CreateDefaultSubobject<UTaskQueueComponent>("TaskQueueComponent");
	SMC = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
	SMC->SetCollisionProfileName("Building");
	SetRootComponent(SMC);
}

void ABuildingActor::OnSelect()
{
	OnBuildingSelected.Execute(BuildingName);
}

void ABuildingActor::RecieveDamage(const ETDOffensiveStatType DmgToTake, const int32 Dmg)
{
	
}

void ABuildingActor::RecieveTask(const TSharedPtr<FTDTask> RecievedTask)
{
	if(RecievedTask->IsQueable())
	{
		TaskQueueComponent->PushTaskToQueue(RecievedTask);
		
		APlayerHud* PlayerHud = Cast<APlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	
	}
}

void ABuildingActor::SetupBuilding(const TObjectPtr<const UBuildingData> BuildingData)
{
	TaskThisBuildingIsAbleToExecute = BuildingData->TaskThisBuildingIsAbleToExecute;
	BuildingName = BuildingData->GetBuildingName();
	
	GetStats()->InitializeStats(BuildingData->DefensiveStats, BuildingData->OffensiveStats);
}


