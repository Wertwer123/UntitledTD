// Fill out your copyright notice in the Description page of Project Settings.
#include "FUnitCollectionPoint.h"

FVector FUnitCollectionPoint::GetUnitMovePointWorld(const FTransform& SourceTransform) const
{
	return SourceTransform.TransformPosition(RelativePositionOfMovePoint);
}

void FUnitCollectionPoint::SetUnitMovePointRelative(const FTransform& SourceTransform, const FVector& NewWorldPos)
{
	RelativePositionOfMovePoint = SourceTransform.InverseTransformPosition(NewWorldPos);
}