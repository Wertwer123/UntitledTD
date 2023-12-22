// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeAbleImage.h"

void UChangeAbleImage::SetImage(const TObjectPtr<UTexture2D> NewImage)
{
	ChangeableImage->SetBrushFromTexture(NewImage);
}
