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
class UDungeonItemsHandler;
class UDungeonBuilderTest;

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDURALDUNGEONS_API UDungeonBuilder : public UObject
{
	GENERATED_BODY()
	
public:
	friend UDungeonBuilderTest;

	UFUNCTION(BlueprintCallable)
	void Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers, UDataTable* InItems, UDungeonItemsHandler* InItemsHandler);

	UFUNCTION(BlueprintCallable)
	void BuildPaperDungeon(FPaperDungeon& PaperDungeon);

	UFUNCTION(BlueprintCallable)
	void BuildDungeon(UPARAM(ref) FPaperDungeon& PaperDungeon, FVector StartLocation);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StartPiece = FName("Room5");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseItems = true;

private:
	/* Recursive function called when BuildPaperDungeon() is called that does the real work of building the Dungeon */
	void CreateNextTiles(FPaperDungeon& PaperDungeon, FPaperDungeonTile& PreviousRoom, FOpening* PreviousOpening, int32 X, int32 Y);

	/* Determines the X and Y difference to the Location of the Socket from the Pivot of the Room, given the Room's rotation */
	FVector GetPlacementOffset(FRotator Rotation, FVector Location);

	/*
	* Calculate the correct rotation of the room that is going to be attached based on the:
	*  Rotation of the Opening on the first Room
	*  Rotation of the Opening on the Room to be spawned
	*  Rotation of the first Room
	*/
	FRotator GetSpawnRotation(FRotator FirstRotation, FRotator SecondRotation, FRotator RoomRotation);

	/*
	* Calculates the X and Y of the next Tile that needs to have a Room placed in it, given
	*  The current Room's rotation
	*  The Opening chosen to lead to the next Room
	*/
	void GetNextXY(FRotator OpeningOrientation, FRotator RoomRotation, int32& NextX, int32& NextY);

	/* Returns the degree given between 0 and 360 */
	float CorrectDegrees(float DegreesToCorrect);

	void CreateItems(FPaperDungeon& PaperDungeon);

	void PlaceRoom(FPaperDungeon& PaperDungeon, FOpening* PreviousOpening, FPaperDungeonTile& PreviousRoom, int32 X, int32 Y, TArray<FOpening*>& Openings);
	void PlaceBarrier(FPaperDungeon& PaperDungeon, FOpening* PreviousOpening, FPaperDungeonTile& PreviousRoom);

	UDungeonItemsHandler* ItemsHandler;
	UDungeonData* DungeonData;
	float DefaultRoomSize = 1000.f;

};
