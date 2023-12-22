// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDGridCollection.h"
#include "GameFramework/Actor.h"
#include "TDGridComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDPATHFINDING_API UTDGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	int32 GetGridDrawWidth() const {return GridDrawWidth;}
	TObjectPtr<UTDGridCollection> GetGrids() const {return  GridToBake;}
	FVector GetGridReferencePosition() const
	{
		if(!ReferencePos) return FVector::Zero();
		
		return  ReferencePos->GetActorLocation();
	}
	
private:
	UTDGridComponent();

	UPROPERTY(EditInstanceOnly, Category= "GridVisualization")
	int32 GridDrawWidth = 50;
	
	UPROPERTY(EditAnywhere, Category= "GridVisualization")
	TObjectPtr<UTDGridCollection> GridToBake = nullptr;

	UPROPERTY(EditAnywhere, Category= "GridVisualization")
	TObjectPtr<AActor> ReferencePos = nullptr;
	
	UFUNCTION(CallInEditor, Category= "GridGeneration")
	void GenerateGrid();
};
