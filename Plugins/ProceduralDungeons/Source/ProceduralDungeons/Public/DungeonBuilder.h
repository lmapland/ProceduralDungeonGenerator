// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/PaperDungeon.h"
#include "Structs/Room.h"
#include "Structs/Opening.h"
#include "DungeonBuilder.generated.h"

class UDataTable;
class UDungeonData;

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDURALDUNGEONS_API UDungeonBuilder : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers);

	UFUNCTION(BlueprintCallable)
	void TestBuildDungeon();

	UFUNCTION(BlueprintCallable)
	void BuildPaperDungeon(FPaperDungeon& PaperDungeon);

	UFUNCTION(BlueprintCallable)
	void BuildDungeon(UPARAM(ref) FPaperDungeon& PaperDungeon, FVector StartLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartingSocket = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialYaw = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingLocation = FVector(0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RoomToSpawn = FName("Room2");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StartPiece = FName("Room5");

private:
	FVector GetPlacementOffset(FRotator Rotation, FVector Location);
	FRotator GetSpawnRotation(FRotator FirstRotation, FRotator SecondRotation, FRotator RoomRotation);
	void CreateNextTiles(FPaperDungeon& PaperDungeon, FPaperDungeonTile& PreviousRoom, FOpening* PreviousOpening, int32 X, int32 Y);
	void GetNextXY(FRotator OpeningOrientation, FPaperDungeonTile& CurrentTile, int32& NextX, int32& NextY);

	/* Returns the degree given between 0 and 360 */
	float CorrectDegrees(float DegreesToCorrect);

	UDataTable* Rooms;
	UDataTable* Openings;
	UDataTable* Barriers;
	UDungeonData* DungeonData;
	float DefaultRoomSize = 1000.f;

};
