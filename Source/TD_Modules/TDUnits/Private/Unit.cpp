#include "Unit.h"
#include "UnitData.h"

// Sets default values
AUnit::AUnit()
{
	StatsComponent = CreateDefaultSubobject<UTDStatsComponent>("BuildingStats");
	TaskQueueComponent = CreateDefaultSubobject<UTaskQueueComponent>("TaskQueueComponent");
	SMC = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
	SMC->SetCollisionProfileName("Unit");
	SetRootComponent(SMC);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnit::RecieveTask(const TSharedPtr<FTDTask> RecievedTask)
{
	TaskQueueComponent->PushTaskToQueue(RecievedTask);
}

void AUnit::RecieveDamage(const ETDOffensiveStatType TypeOfDmg, const int32 Dmg)
{
	FTDOffensiveStat FoundStat;
	
	if(StatsComponent->TryFindMutableOffensiveStat(TypeOfDmg, FoundStat))
	{
		switch (FoundStat.GetStatType())
		{
		case ETDOffensiveStatType::Empty:
			break;
		case ETDOffensiveStatType::AttackDmg:
			break;
		case ETDOffensiveStatType::Range:
			break;
		case ETDOffensiveStatType::ArmorPierce:
			break;
		case ETDOffensiveStatType::Health:
			break;
		default: ;
		}
	}
}

void AUnit::OnSelect()
{
	OnUnitSelected.Execute(UnitName);
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnit::SetupUnit(const TObjectPtr< const UUnitData>& UnitData)
{
	UnitName = UnitData->GetUnitName();
	TaskThisUnitIsAbleToExecute = UnitData->GetTasksThisUnitIsAbleToExecute();

	StatsComponent->InitializeStats(UnitData->GetDefensiveStats(), UnitData->GetOffensiveStats());
}

