#pragma once
#include "CoreMinimal.h"
#include "ResourceBuildingActor.h"
#include "HQBuilding.generated.h"


UCLASS()
class BUILDING_API AHQBuilding : public AResourceBuildingActor
{
	GENERATED_BODY()

public:
	AHQBuilding();

private:
	
	virtual void SetupBuilding(const TObjectPtr<const UBuildingData> BuildingData) override;
};
