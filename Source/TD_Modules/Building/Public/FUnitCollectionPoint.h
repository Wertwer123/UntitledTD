#pragma once
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "FUnitCollectionPoint.generated.h"

USTRUCT()
struct BUILDING_API FUnitCollectionPoint
{
	GENERATED_BODY()
public:
	FUnitCollectionPoint(){}

public:
	FVector GetUnitMovePointWorld(const FTransform& SourceTransform) const;
	void SetUnitMovePointRelative(const FTransform& SourceTransform, const FVector& NewWorldPos);

private:
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget), Category= "MovePoint")
	FVector RelativePositionOfMovePoint = FVector::Zero();
};
