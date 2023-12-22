#include "Buildings/HQBuilding.h"

#include "TaskQueueComponent.h"

// Sets default values
AHQBuilding::AHQBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHQBuilding::SetupBuilding(const TObjectPtr<const UBuildingData> BuildingData)
{
	Super::SetupBuilding(BuildingData);
}

