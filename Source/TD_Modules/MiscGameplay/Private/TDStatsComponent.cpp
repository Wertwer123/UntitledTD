// Fill out your copyright notice in the Description page of Project Settings.


#include "TDStatsComponent.h"


#pragma region StatTypeHashing
uint32 GetTypeHash(const FTDDefensiveStat& DefensiveStat)
{
	return FCrc::MemCrc32(&DefensiveStat, sizeof(DefensiveStat));
}

uint32 GetTypeHash(const FTDOffensiveStat& OffensiveStat)
{
	return FCrc::MemCrc32(&OffensiveStat, sizeof(OffensiveStat));
}


#pragma endregion //Stat type hashing
// Sets default values for this component's properties
UTDStatsComponent::UTDStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTDStatsComponent::InitializeStats(const TSet<FTDDefensiveStat>& InDefensiveStats,
	const TSet<FTDOffensiveStat>& InOffensiveStats)
{
	DefensiveStats = InDefensiveStats;
	OffensiveStats = InOffensiveStats;

	for (FTDDefensiveStat& DefensiveStat : DefensiveStats)
	{
		DefensiveStat.SetStat(DefensiveStat.GetDefaultResistance());
	}
	for (FTDOffensiveStat& OffensiveStat : OffensiveStats)
	{
		OffensiveStat.SetStat(OffensiveStat.GetMaxStatValue());
	}
}

bool UTDStatsComponent::TryFindMutableOffensiveStat(ETDOffensiveStatType TypeOfStatToFind, FTDOffensiveStat& OutStat)
{
	for (FTDOffensiveStat& OffensiveStat : OffensiveStats)
	{
		if(OffensiveStat.GetStatType() == TypeOfStatToFind)
		{
			OutStat = OffensiveStat;
			return true;
		}
	}

	return false;
}

bool UTDStatsComponent::TryFindMutableDefensiveStat(ETDDefensiveStatType TypeOfStatToFind, FTDDefensiveStat& OutStat)
{
	
	for (FTDDefensiveStat& DefensiveStat : DefensiveStats)
	{
		if(DefensiveStat.GetStatType() == TypeOfStatToFind)
		{
			OutStat = DefensiveStat;
			return true;
		}
	}

	return false;
}

