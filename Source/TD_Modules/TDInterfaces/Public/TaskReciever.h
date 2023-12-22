#pragma once
#include "CoreMinimal.h"
#include "TDTaskDescription.h"
#include "UObject/Interface.h"
#include "TaskReciever.generated.h"

UINTERFACE()
class TDINTERFACES_API UTaskReciever : public UInterface
{
	GENERATED_BODY()
};

class TDINTERFACES_API ITaskReciever
{
	GENERATED_BODY()

public:
	virtual void RecieveTask(const TSharedPtr<FTDTask> RecievedTask){};

};
