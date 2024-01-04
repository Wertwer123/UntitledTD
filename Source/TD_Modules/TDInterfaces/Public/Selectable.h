#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

UINTERFACE()
class TDINTERFACES_API USelectable : public UInterface
{
	GENERATED_BODY()
};

class TDINTERFACES_API ISelectable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnSelect(){};

	UFUNCTION()
	virtual void OnDeselect(){};
};
