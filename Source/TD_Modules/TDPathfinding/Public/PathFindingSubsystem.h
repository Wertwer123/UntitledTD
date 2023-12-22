// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStarThread.h"
#include "TDGridCollection.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathFindingSubsystem.generated.h"

UCLASS()
class TDPATHFINDING_API UPathFindingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UTDGridCollection> GridCollection;
	
	void RequestAStarPath();

private:
	AStarThread* AStarWorker = nullptr;

	virtual void Deinitialize() override;
	void StartAStarThread();
	void StopAStarThread();
	void OnWorldBeginPlay(UWorld& InWorld) override;
};
