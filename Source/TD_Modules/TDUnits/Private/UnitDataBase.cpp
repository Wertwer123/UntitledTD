// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitDataBase.h"

TArray<FName> UUnitDataBase::GetAllUnitNames()
{
	TArray<FName> UnitNames;
	
	for (const TObjectPtr<UUnitData>& UnitData : AllUnits)
	{
		UnitNames.Add(UnitData->GetUnitName());
	}

	return UnitNames;
}
