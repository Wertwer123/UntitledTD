#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "PlayerHudUI.generated.h"

class UTaskGrid;
class USelectionInformationDisplay;

UCLASS()
class TD_UI_API UPlayerHudUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "HUD")
	TObjectPtr<UCanvasPanel> CanvasPanel = nullptr;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category= "HUD")
	TObjectPtr<UTaskGrid> TaskGrid;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category= "HUD")
	TObjectPtr<USelectionInformationDisplay> SelectionInformationDisplay;
};
