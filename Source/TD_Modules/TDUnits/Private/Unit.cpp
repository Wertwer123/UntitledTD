#include "Unit.h"

#include "AStarPathRequest.h"
#include "MoveUnitTaskDescription.h"
#include "PathFindingSubsystem.h"
#include "UnitData.h"

// Sets default values
AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	StatsComponent = CreateDefaultSubobject<UTDStatsComponent>("BuildingStats");
	TaskQueueComponent = CreateDefaultSubobject<UTaskQueueComponent>("TaskQueueComponent");
	SMC = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
	SMC->SetCollisionProfileName("Unit");
	SetRootComponent(SMC);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	if(UWorld* World = GetWorld())
	{
		if(UPathFindingSubsystem* PathFindingSubsystem = World->GetSubsystem<UPathFindingSubsystem>())
		{
			const FAStarPathRequest PathRequest = PathFindingSubsystem->CreateRequest(GetActorLocation(), FVector(100000, 100000, 0));
		
			PathFindingSubsystem->RequestHPAStarPath(PathRequest, this);
		}
	}
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

void AUnit::RecieveHPAStarPath(FHpaStarPath& HPAStarPathfindingResult)
{
	PathToFollow = HPAStarPathfindingResult;
}

void AUnit::MoveOnPath()
{
	if(!PathToFollow.PositionsToWalk.IsEmpty())
	{
		const FVector& Target = PathToFollow.PositionsToWalk[CurrentPathIndex];
		const FVector& MovingDirection = (Target - GetActorLocation()).GetSafeNormal() * FApp::GetDeltaTime();
		SetActorLocation(GetActorLocation() + MovingDirection * 1100);

		if(FVector::DistSquared(Target, GetActorLocation()) < 20 * 20)
		{
			CurrentPathIndex++;
			if(CurrentPathIndex == PathToFollow.PositionsToWalk.Num() - 1)
			{
				PathToFollow.PositionsToWalk.Empty();
			}
		}
	}
	
}

void AUnit::OnSelect()
{
	bIsSelected = true;
	OnUnitSelected.Execute(UnitName);
}

void AUnit::OnDeselect()
{
	bIsSelected = false;
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveOnPath();
}

void AUnit::SetupUnit(const TObjectPtr< const UUnitData>& UnitData)
{
	UnitName = UnitData->GetUnitName();
	TaskThisUnitIsAbleToExecute = UnitData->GetTasksThisUnitIsAbleToExecute();

	StatsComponent->InitializeStats(UnitData->GetDefensiveStats(), UnitData->GetOffensiveStats());
}

