#pragma once

#include "CoreMinimal.h"
#include "PaperDungeonTile.h"
#include "PaperDungeon.generated.h"

USTRUCT(Blueprintable)
struct FPaperDungeon
{
	GENERATED_BODY()

	TArray<TArray<FPaperDungeonTile>> Layout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DungeonSize = 10;

	/* The first Room should beplaced on the fifth column of the third row */
	FIntVector StartLocation = FIntVector(7, 4, 0);

	bool IsReady = false;

	TArray<FPaperDungeonTile> Barriers;

	void InitializeArray(int32 DefaultSize = 10)
	{
		DungeonSize = DefaultSize;
		// Initialize the Array of Arrays
		FPaperDungeonTile Tile;
		TArray<FPaperDungeonTile> Tmp;
		Tmp.Init(Tile, DungeonSize);
		Layout.Init(Tmp, DungeonSize);

		IsReady = true;
	}

	void UpdateTile(int32 X, int32 Y, float LocX, float LocY, float LocZ, float Pitch, float Yaw, float Roll, FName RoomName)
	{
		Layout[X][Y].IsOccupied = true;
		Layout[X][Y].RoomName = RoomName;
		Layout[X][Y].Location.X = LocX;
		Layout[X][Y].Location.Y = LocY;
		Layout[X][Y].Location.Z = LocZ;
		Layout[X][Y].Rotation.Pitch = Pitch;
		Layout[X][Y].Rotation.Yaw = Yaw;
		Layout[X][Y].Rotation.Roll = Roll;
	}

	void AddBarrier(float X, float Y, float Z, float Pitch, float Yaw, float Roll, FName BarrierName)
	{
		FPaperDungeonTile Tile;
		Tile.Location.X = X;
		Tile.Location.Y = Y;
		Tile.Location.Z = Z;
		Tile.Rotation.Pitch = Pitch;
		Tile.Rotation.Yaw = Yaw;
		Tile.Rotation.Roll = Roll;
		Tile.RoomName = BarrierName;
		Barriers.Add(Tile);
	}

	void Print()
	{
		if (!IsReady) return;

		UE_LOG(LogTemp, Warning, TEXT("Printing FPaperDungeon structure"));
		for (const TArray<FPaperDungeonTile>& Row : Layout)
		{
			FString RowString = FString("");
			for (const FPaperDungeonTile& Tile : Row)
			{
				RowString += Tile.IsOccupied ? FString("[x]") : FString("[ ]");
			}
			UE_LOG(LogTemp, Warning, TEXT("%s"), *RowString);
		}
	}
};