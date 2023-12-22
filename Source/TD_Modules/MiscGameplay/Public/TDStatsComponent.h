#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Set.h"
#include "TDStatsComponent.generated.h"
#pragma region StatTypes

UENUM()
enum class ETDOffensiveStatType : uint8
{
	Empty,
	AttackDmg,
	Range,
	ArmorPierce,
	Health,
};

UENUM()
enum class ETDDefensiveStatType : uint8
{
	Empty,
	Armor,
	AcidResistance,
	PiercingResistance,
};

USTRUCT(BlueprintType)
struct MISCGAMEPLAY_API FTDOffensiveStat
{
	GENERATED_BODY()

public:

	FTDOffensiveStat()
	{
		CurrentStatValue = MaxStatValue;
	}

	void SetStat(const uint32 NewValue) {CurrentStatValue = NewValue;}
	
	ETDOffensiveStatType GetStatType() const {return TypeOfStat;}
	uint8 GetMaxStatValue() const {return MaxStatValue;}
	uint8 GetCurrentStatValue() const {return CurrentStatValue;}
	
	bool operator==(const FTDOffensiveStat& Other) const
	{
		return Other.TypeOfStat == TypeOfStat;
	}
	
protected:
	UPROPERTY(EditAnywhere, Category= "Stat")
	ETDOffensiveStatType TypeOfStat = ETDOffensiveStatType::Empty;

	UPROPERTY(EditAnywhere, Category= "Stat")
	uint32 MaxStatValue = 10;
	
	UPROPERTY(VisibleAnywhere, Category= "Stat")
	uint32 CurrentStatValue = 5;
};

USTRUCT()
struct MISCGAMEPLAY_API FTDDefensiveStat
{
	GENERATED_BODY()

public:

	FTDDefensiveStat()
	{
		CurrentResistancePercentage = DefaultResistancePercentage;
	}

	void SetStat(const uint32 NewValue) {CurrentResistancePercentage = NewValue;}
	ETDDefensiveStatType GetStatType() const {return TypeOfStat;}
	FORCEINLINE uint32 GetDefaultResistance() const {return DefaultResistancePercentage;}
	FORCEINLINE uint32 GetCurrentResistance() const {return CurrentResistancePercentage;}

	bool operator==(const FTDDefensiveStat& Other) const
	{
		return Other.TypeOfStat == TypeOfStat;
	}
	
	//Returns the multiplier for a given dmg value
	float GetResistancePercentage()
	{
		const float Perentage = static_cast<float>(CurrentResistancePercentage / 100);
		const float Multiplier = 1 - Perentage;
		return Multiplier;
	}
	
protected:
	UPROPERTY(EditAnywhere, Category= "Stat")
	ETDDefensiveStatType TypeOfStat = ETDDefensiveStatType::Empty;

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0, ClampMax = 100, Delta = 1), Category= "Stat")
	uint32 DefaultResistancePercentage = 20;
	
	UPROPERTY(VisibleAnywhere, Category= "Stat")
	uint32 CurrentResistancePercentage = 10;
};

uint32 GetTypeHash(const FTDDefensiveStat& DefensiveStat);
uint32 GetTypeHash(const FTDOffensiveStat& OffensiveStat);

#pragma endregion //Stat types end


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOffensiveStatUpdated, const FTDOffensiveStat&, UpdatedStat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDefensiveStatUpdated, const FTDDefensiveStat&, UpdatedStat);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MISCGAMEPLAY_API UTDStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDStatsComponent();

	UPROPERTY()
	FOnOffensiveStatUpdated OnOffensiveStatUpdated;
	
	UPROPERTY()
	FOnDefensiveStatUpdated OnDefensiveStatUpdated;
	
	void InitializeStats(const TSet<FTDDefensiveStat>& InDefensiveStats, const TSet<FTDOffensiveStat>& InOffensiveStats);
	
	//TODO not a hundred percent sure if we are getting the actual stats here
	bool TryFindMutableOffensiveStat(ETDOffensiveStatType TypeOfStatToFind, FTDOffensiveStat& OutStat);
	bool TryFindMutableDefensiveStat(ETDDefensiveStatType TypeOfStatToFind, FTDDefensiveStat& OutStat);
	
	TSet<FTDOffensiveStat>& GetAllOffensiveStats() {return OffensiveStats;}
	TSet<FTDDefensiveStat>& GetAllDefensiveStats() {return DefensiveStats;}
	
protected:
	UPROPERTY(VisibleAnywhere, Category= "Stats")
	TSet<FTDOffensiveStat> OffensiveStats{};

	UPROPERTY(VisibleAnywhere, Category= "Stats")
	TSet<FTDDefensiveStat> DefensiveStats{};
};
