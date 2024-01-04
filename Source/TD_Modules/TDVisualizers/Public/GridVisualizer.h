#pragma once
#include "ComponentVisualizer.h"
#include "TDGridCollection.h"
#include "TDGridComponent.h"

class TDVISUALIZERS_API FGridVisualizer : public FComponentVisualizer
{
public:
	void DrawGrid(const FGrid& GridToDraw, const UTDGridComponent* GridVisualizationComponent, const FColor& GridColor, FPrimitiveDrawInterface* PDI);
	void DrawGridPosition(const FGrid& GridToDrawPositionIn, const UTDGridComponent* GridVisualizationComponent, FPrimitiveDrawInterface* PDI);
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

private:
	void DrawClusterExits(const FCluster& InCLusterToDrawConnectionsFor, const UTDGridComponent* GridComponent, FPrimitiveDrawInterface* PDI);
};
