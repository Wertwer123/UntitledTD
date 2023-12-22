// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

enum class ETDOffensiveStatType : uint8;

// This class does not need to be modified.
UINTERFACE()
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TDINTERFACES_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void RecieveDamage(const ETDOffensiveStatType TypeOfDmg, const int32 Dmg){}	
};
