#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TDGridCollection.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(TDWorldGrids, Log, All)

UENUM()
enum EGridType
{
	Base,
	FlowField
};

USTRUCT()
struct TDPATHFINDING_API FGridCell
{
	GENERATED_BODY()

	FGridCell(){}
	FGridCell(
		const FVector& InVector,
		const int32 InCellIndex,
		const int32 InRowIndex,
		const int32 InColumnIndex) :
	
		Center(InVector),
		CellIndex(InCellIndex),
		RowIndex(InRowIndex),
		ColumnIndex(InColumnIndex){}

	bool operator==(const FGridCell& Other) const
	{
		return RowIndex == Other.RowIndex && ColumnIndex == Other.ColumnIndex;
	}
	
	UPROPERTY()
	FVector Center = FVector::Zero();

	UPROPERTY()
	int32 CellIndex = 0;
	
	UPROPERTY()
	int32 RowIndex = 0;

	UPROPERTY()
	int32 ColumnIndex = 0;
};

USTRUCT()
struct TDPATHFINDING_API FGrid
{
	GENERATED_BODY()

	virtual ~FGrid(){}
	
private:
	UPROPERTY(EditAnywhere, Category= "Grid|Settings")
	int32 CellExtents = 100;
	
	UPROPERTY(EditAnywhere, Category= "Grid|Settings")
	int32 GridWidth = 100;

	UPROPERTY(EditAnywhere, Category= "Grid|Settings")
	int32 GridHeight = 100;
	
	UPROPERTY(VisibleAnywhere, Category= "Grid")
	TArray<FGridCell> Cells;

	UPROPERTY(VisibleAnywhere, Category= "Grid|Information")
	int32 Columns = 0;
	
	UPROPERTY(VisibleAnywhere, Category= "Grid|Information")
	int32 Rows = 0;
	
public:
	void EmptyGrid();
	void AddGridCell(const FGridCell& CellToAdd);
	void SetRows(const int32 InRows);
	void SetColumns(const int32 InColumns);
	virtual void GenerateGrid();

	int32 GetGridWidth () const;
	int32 GetGridHeight() const;
	int32 GetCellExtents() const {return CellExtents;}
	int32 GetColumns() const {return Columns;}
	int32 GetRows() const {return Rows;}
	TArray<FGridCell> GetNeighborsOfCell(const FGridCell& GridCellToGetNeighborsFrom) const;
	const FGridCell& GetCellAtPosition(const FVector& PositionToGetCellAt) const;
	const TArray<FGridCell>& GetCells() const {return Cells;} 
};
UCLASS()
class TDPATHFINDING_API UTDGridCollection : public UDataAsset
{
	GENERATED_BODY()
public:
	void GenerateGrids();
	FGrid& GetBaseGrid() {return BaseGrid;}

private:
	UPROPERTY(EditDefaultsOnly, Category= "PathfindingGrid")
	FGrid BaseGrid;
};
