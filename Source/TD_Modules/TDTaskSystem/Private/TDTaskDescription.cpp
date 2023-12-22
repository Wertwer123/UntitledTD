#include "..\Public\TDTaskDescription.h"

TSharedPtr<FTDTask> UTaskDescription::CreateTaskObject(const TObjectPtr<UObject> Executor)
{
	TSharedPtr<FTDTask> NewTask = MakeShared<FTDTask>();
	
	NewTask->InitTask(Executor, TaskDisplay, bIsQueable);
	return NewTask; 
}
