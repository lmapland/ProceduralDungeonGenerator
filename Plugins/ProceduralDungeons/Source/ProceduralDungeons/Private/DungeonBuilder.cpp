// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonBuilder.h"
#include "Engine/DataTable.h"
#include "DungeonItemsHandler.h"
#include "DungeonData.h"
#include "TileBase.h"

void UDungeonBuilder::Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers, UDataTable* InItems, UDungeonItemsHandler* InItemsHandler)
{
	ItemsHandler = InItemsHandler;
	
	DungeonData = NewObject<UDungeonData>();
	DungeonData->Setup(InRooms, InOpenings, InBarriers, InItems);
}

void UDungeonBuilder::BuildPaperDungeon(FPaperDungeon& PaperDungeon)
{
	PaperDungeon.InitializeArray();

	// "Place down" start piece
	int32 X = PaperDungeon.StartLocation.X;
	int32 Y = PaperDungeon.StartLocation.Y;
	PaperDungeon.Layout[X][Y].RoomName = StartPiece;
	PaperDungeon.Layout[X][Y].IsOccupied = true;

	// Get and apply new Rooms for all Openings off of StartPiece
	TArray<FOpening*> RoomOpenings = DungeonData->GetOpeningsFor(StartPiece);
	for (FOpening* Opening : RoomOpenings)
	{
		int32 NextX = X;
		int32 NextY = Y;

		GetNextXY(Opening->Orientation, PaperDungeon.Layout[X][Y].Rotation, NextX, NextY);
		CreateNextTiles(PaperDungeon, PaperDungeon.Layout[X][Y], Opening, NextX, NextY);
	}

	PaperDungeon.Print();

	CreateItems(PaperDungeon);
}

/*
* When CreateNextTiles is called, a room has been chosen and placed down.
*  The Tile has been updated with all of the Room's information
*  An Opening has been chosen
*  The new Tile Location (the X and Y within the PaperDungeon's 2D Layout TArray) has been chosen
* CreateNextTiles() is passed all of this information
*  It returns if the Tile is already occupied or if the X or Y are out of bounds
*  Otherwise, it selects the next Room to place and chooses the Opening for that Room
*  It removes the selected Opening from the TArray of Openings
*  It loops through any remaining Openings, determines the new X & Y for the next Room
*/
void UDungeonBuilder::CreateNextTiles(FPaperDungeon& PaperDungeon, FPaperDungeonTile& PreviousRoom, FOpening* PreviousOpening, int32 X, int32 Y)
{
	// Base cases / bounds checking
	if (X < 0 || Y < 0 || X >= PaperDungeon.DungeonSize || Y >= PaperDungeon.DungeonSize || PaperDungeon.Layout[X][Y].IsOccupied)
	{
		if (LogLevel >= 4)
		{
			if (X < 0) { UE_LOG(LogTemp, Display, TEXT("X < 0 (%i)"), X); }
			else if (Y < 0) { UE_LOG(LogTemp, Display, TEXT("Y < 0 (%i)"), Y); }
			else if (X >= PaperDungeon.DungeonSize) { UE_LOG(LogTemp, Display, TEXT("X >= PaperDungeon.DungeonSize (%i >= %i)"), X, PaperDungeon.DungeonSize); }
			else if (Y >= PaperDungeon.DungeonSize) { UE_LOG(LogTemp, Display, TEXT("Y >= PaperDungeon.DungeonSize (%i >= %i)"), Y, PaperDungeon.DungeonSize); }
			else if (PaperDungeon.Layout[X][Y].IsOccupied) { UE_LOG(LogTemp, Display, TEXT("PaperDungeon.Layout[X][Y].IsOccupied (%i, %i)"), X, Y); }
		}

		// There is an Opening and we can't place another room beyond it. Place a Barrier instead
		PlaceBarrier(PaperDungeon, PreviousOpening, PreviousRoom);
		return;
	}

	TArray<FOpening*> CurrentOpenings;
	PlaceRoom(PaperDungeon, PreviousOpening, PreviousRoom, X, Y, CurrentOpenings);

	// Get and place new Rooms for all Openings off of CurrentRoom
	for (FOpening* Opening : CurrentOpenings)
	{
		int32 NextX = X;
		int32 NextY = Y;

		GetNextXY(Opening->Orientation, PaperDungeon.Layout[X][Y].Rotation, NextX, NextY);
		CreateNextTiles(PaperDungeon, PaperDungeon.Layout[X][Y], Opening, NextX, NextY);
	}
}

void UDungeonBuilder::BuildDungeon(FPaperDungeon& PaperDungeon, FVector StartLocation)
{
	// Spawn Dungeon Rooms from the PaperDungeon Tiles
	for (const TArray<FPaperDungeonTile>& Row : PaperDungeon.Layout)
	{
		for (const FPaperDungeonTile& Tile : Row)
		{
			if (Tile.IsOccupied)
			{
				FRoom* Room = DungeonData->GetRoom(Tile.RoomName);
				GetWorld()->SpawnActor<ATileBase>(Room->bp_ref, Tile.Location, Tile.Rotation);
			}
		}
	}

	// Spawn any necessary Barriers
	for (const FPaperDungeonTile& Tile : PaperDungeon.Barriers)
	{
		FBarrier* Barrier = DungeonData->GetBarrier(Tile.RoomName);
		GetWorld()->SpawnActor<AActor>(Barrier->bp_ref, Tile.Location, Tile.Rotation);
	}
}

// Rotation of Room
// Location of Socket
FVector UDungeonBuilder::GetPlacementOffset(FRotator Rotation, FVector Location)
{
	FVector LocationToReturn(Location.X, Location.Y, Location.Z);

	if (FMath::IsNearlyEqual(Rotation.Yaw, -360.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 0.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 360.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Offset: Yaw is 0"));
		return LocationToReturn;
	}
	else if (FMath::IsNearlyEqual(Rotation.Yaw, -270.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 90.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Offset: Yaw is 90"));
		LocationToReturn.X = Location.Y;
		LocationToReturn.Y = Location.X;
		return LocationToReturn * FVector(-1.f, 1.f, 1.f);
	}
	else if (FMath::IsNearlyEqual(Rotation.Yaw, 180.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 540.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Offset: Yaw is 180"));
		return LocationToReturn * FVector(-1.f, -1.f, 1.f);
	}
	else if (FMath::IsNearlyEqual(Rotation.Yaw, -90.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 270.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Offset: Yaw is 270"));
		LocationToReturn.X = Location.Y;
		LocationToReturn.Y = Location.X;
		return LocationToReturn * FVector(1.f, -1.f, 1.f);
	}
	else
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Offset: Yaw not found: %f"), Rotation.Yaw);
	}

	return LocationToReturn;
}

FRotator UDungeonBuilder::GetSpawnRotation(FRotator FirstRotation, FRotator SecondRotation, FRotator RoomRotation)
{
	if (LogLevel >= 5) UE_LOG(LogTemp, Display, TEXT("GetSpawnRotation(): FirstRot, SecondRot, RoomRot: %f, %f, %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw);
	FRotator OpeningOrientation = FirstRotation + RoomRotation;

	// Get the values between 0 and 360 so the math works well
	OpeningOrientation.Yaw = CorrectDegrees(OpeningOrientation.Yaw);

	FRotator RotationToSpawn = SecondRotation - OpeningOrientation;
	if (FMath::IsNearlyEqual(OpeningOrientation.Yaw, SecondRotation.Yaw, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Yaws are equal; setting rotation to 180.f"));
		RotationToSpawn.Yaw = 180.f;
	}
	else if (FMath::IsNearlyEqual(FMath::Abs<float>(OpeningOrientation.Yaw - SecondRotation.Yaw), 180.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Yaws are 180.f apart; will set Rotation to 0.f"));
		RotationToSpawn.Yaw = 0.f;
	}

	RotationToSpawn.Yaw = CorrectDegrees(RotationToSpawn.Yaw);
	if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("Orientation to Rotate: [%f, %f, %f]"), RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll);
	return RotationToSpawn;
}

void UDungeonBuilder::GetNextXY(FRotator OpeningOrientation, FRotator RoomRotation, int32& NextX, int32& NextY)
{
	FRotator AbsoluteRotation = OpeningOrientation + RoomRotation;
	AbsoluteRotation.Yaw = CorrectDegrees(AbsoluteRotation.Yaw);
	if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("AbsoluteRotation.Yaw: %f"), AbsoluteRotation.Yaw);

	if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 0.f, 1.f)) NextY -= 1;
	else if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 90.f, 1.f)) NextX -= 1;
	else if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 180.f, 1.f)) NextY += 1;
	else if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 270.f, 1.f)) NextX += 1;
}

float UDungeonBuilder::CorrectDegrees(float DegreesToCorrect)
{
	if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("CorrectDegrees(): DegreesToCorrect: %f"), DegreesToCorrect);
	float Yaw = DegreesToCorrect;

	while (Yaw > 360.f || FMath::IsNearlyEqual(Yaw, 360.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("CorrectDegrees(): Yaw is >= 360; subtracting 360"));
		Yaw -= 360.f;
	}
	if (FMath::IsNearlyEqual(Yaw, -0.f, 1.f))
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("CorrectDegrees(): Yaw is -0; setting to 0"));
		Yaw = 0.f;
	}
	else if (Yaw < 0)
	{
		if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("CorrectDegrees(): Yaw is negative; adding 360"));
		Yaw += 360.f;
	}

	if (LogLevel >= 4) UE_LOG(LogTemp, Display, TEXT("CorrectDegrees(): Returning %f "), Yaw);
	return Yaw;
}

void UDungeonBuilder::CreateItems(FPaperDungeon& PaperDungeon)
{
	// Check that the DungeonItems DT is not empty: if it is; no need to do any of this.
	if (!UseItems) return;
	if (DungeonData->GetItemArraySize() == 0) return;

	for (const TArray<FPaperDungeonTile>& Row : PaperDungeon.Layout)
	{
		for (const FPaperDungeonTile& Tile : Row)
		{
			if (!Tile.IsOccupied) continue;

			FRoom* Room = DungeonData->GetRoom(Tile.RoomName);

			for (FString ItemName : Room->ItemNames)
			{
				FDungeonItem* Item = DungeonData->GetItem(FName(ItemName));
				if (Item == nullptr) continue;

				float ToSpawn = FMath::RandRange(0.f, 1.f);
				if (ToSpawn > Item->SpawnLikelihood) continue;

				FRotator Rotation = Tile.Rotation + Item->Rotation;
				Rotation.Yaw = CorrectDegrees(Rotation.Yaw);

				FVector Location = Tile.Location + GetPlacementOffset(Tile.Rotation, Item->Location);

				ItemsHandler->HandleNewItem(Item->Key, Location, Rotation);
			}
		}
	}
}

void UDungeonBuilder::PlaceRoom(FPaperDungeon& PaperDungeon, FOpening* PreviousOpening, FPaperDungeonTile& PreviousRoom, int32 X, int32 Y, TArray<FOpening*>& Openings)
{
	FRoom* CurrentRoom = DungeonData->GetRandomRoom();
	Openings = DungeonData->GetOpeningsFor(CurrentRoom->RowName);
	int32 OpeningToUseIndex = FMath::Floor(FMath::RandRange(0, Openings.Num() - 1));
	FRotator RotationToSpawn = GetSpawnRotation(PreviousOpening->Orientation, Openings[OpeningToUseIndex]->Orientation, PreviousRoom.Rotation);
	FVector LocationToSpawn = PreviousRoom.Location + GetPlacementOffset(PreviousRoom.Rotation, PreviousOpening->Location) * -1 + GetPlacementOffset(RotationToSpawn, Openings[OpeningToUseIndex]->Location);

	PaperDungeon.UpdateTile(X, Y, LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z, RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll, CurrentRoom->RowName);

	Openings.RemoveAt(OpeningToUseIndex); // This is the Opening back to PreviousRoom; don't try to spawn a new Room here

}

void UDungeonBuilder::PlaceBarrier(FPaperDungeon& PaperDungeon, FOpening* PreviousOpening, FPaperDungeonTile& PreviousRoom)
{
	FBarrier* Barrier = DungeonData->GetRandomBarrier();

	FRotator RotationToSpawn = GetSpawnRotation(PreviousOpening->Orientation, FRotator(0.f), PreviousRoom.Rotation);
	FVector LocationToSpawn = PreviousRoom.Location + GetPlacementOffset(PreviousRoom.Rotation, PreviousOpening->Location) * -1;
	PaperDungeon.AddBarrier(LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z, RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll, Barrier->RowName);
}
