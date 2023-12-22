#include "TDGridComponent.h"

UTDGridComponent::UTDGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTDGridComponent::GenerateGrid()
{
	FlushPersistentDebugLines(GetWorld());
	if(GridToBake)
	{
		GridToBake->GenerateGrids();
	}
}