#include "TDVisualizers.h"

#include "TDGridComponent.h"
#include "GridVisualizer.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "FTDVisualizersModule"

void FTDVisualizersModule::StartupModule()
{
	if(GUnrealEd != NULL)
	{
		const TSharedPtr<FGridVisualizer> Visualizer = MakeShareable(new FGridVisualizer());
		GUnrealEd->RegisterComponentVisualizer(UTDGridComponent::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}
}

void FTDVisualizersModule::ShutdownModule()
{
	if(GUnrealEd != NULL)
	{
		GUnrealEd->UnregisterComponentVisualizer(UTDGridComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FTDVisualizersModule, TDVisualizers)