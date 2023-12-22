#pragma once

#include "CoreMinimal.h"
#include "UnitData.h"
#include "Engine/DataAsset.h"
#include "UnitDataBase.generated.h"

UCLASS()
class TDUNITS_API UUnitDataBase : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category= "Units")
	TArray<TObjectPtr<UUnitData>> AllUnits;

public:
	TArray<FName> GetAllUnitNames();
	
	FORCEINLINE TObjectPtr<const UUnitData> GetUnit (const FName& UnitToGet) const
	{
		const auto UnitData = AllUnits.FindByPredicate([&UnitToGet](const TObjectPtr<UUnitData>& UnitA)
	   {
			return UnitA->GetUnitName().IsEqual(UnitToGet);
	   });

		return *UnitData;
	}
};
