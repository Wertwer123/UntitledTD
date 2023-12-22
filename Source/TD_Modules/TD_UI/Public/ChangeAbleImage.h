// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ChangeAbleImage.generated.h"

/**
 * 
 */
UCLASS()
class TD_UI_API UChangeAbleImage : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImage(const TObjectPtr<UTexture2D> NewImage);
	
protected:
	UPROPERTY(VisibleAnywhere, meta =(BindWidget), Category= "Image")
	TObjectPtr<UImage> ChangeableImage;
};
