// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonBuilder.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/DataTable.h"
#include "DungeonData.h"
#include "TileBase.h"

#include "DrawDebugHelpers.h"

void UDungeonBuilder::Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers)
{
	Rooms = InRooms;
	Openings = InOpenings;
	Barriers = InBarriers;
	
	DungeonData = NewObject<UDungeonData>();
	DungeonData->Setup(Rooms, Openings, Barriers);
}

void UDungeonBuilder::TestBuildDungeon()
{
	FRoom* Room = DungeonData->GetRoom(StartPiece);

	// Place down the very first room - the Start Piece
	FRotator InitialRotation(0.f, InitialYaw, 0.f);
	ATileBase* SpawnedRoom = GetWorld()->SpawnActor<ATileBase>(Room->bp_ref, StartingLocation, InitialRotation);
	TArray<FOpening*> RoomOpenings = DungeonData->GetOpeningsFor(StartPiece);

	FRoom* Room2 = DungeonData->GetRoom(RoomToSpawn);
	TArray<FOpening*> RoomOpenings2 = DungeonData->GetOpeningsFor(Room2->RowName);
	if (RoomOpenings2.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomOpenings2.Num() is empty!"));
		return;
	}

	FVector RoomLocation = SpawnedRoom->GetActorLocation();
	FVector SocketOffset = GetPlacementOffset(InitialRotation, RoomOpenings[StartingSocket]->Location) * -1;

	UE_LOG(LogTemp, Warning, TEXT("--- Socket: %i, Yaw: %f ---"), StartingSocket, InitialYaw);
	UE_LOG(LogTemp, Warning, TEXT("Room1 Orientation: [%f, %f, %f]"), SpawnedRoom->GetActorRotation().Pitch, SpawnedRoom->GetActorRotation().Yaw, SpawnedRoom->GetActorRotation().Roll);

	FRotator RotationToSpawn = GetSpawnRotation(RoomOpenings[StartingSocket]->Orientation, RoomOpenings2[0]->Orientation, SpawnedRoom->GetActorRotation());
	DrawDebugSphere(GetWorld(), RoomLocation + SocketOffset, 20.f, 4, FColor::Red, true, -1.f, (uint8)0U, 1.f);

	UE_LOG(LogTemp, Warning, TEXT("Opening1 Location: [%f, %f, %f]"), (RoomLocation + SocketOffset).X, (RoomLocation + SocketOffset).Y, (RoomLocation + SocketOffset).Z);
	FVector LocationOffset = GetPlacementOffset(RotationToSpawn, RoomOpenings2[0]->Location);
	UE_LOG(LogTemp, Warning, TEXT("LocationOffset: [%f, %f, %f]"), LocationOffset.X, LocationOffset.Y, LocationOffset.Z);

	FVector LocationToSpawn = RoomLocation + SocketOffset + LocationOffset;
	UE_LOG(LogTemp, Warning, TEXT("Final Location: [%f, %f, %f]"), LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z);

	ATileBase* SpawnedRoom2 = GetWorld()->SpawnActor<ATileBase>(Room2->bp_ref, LocationToSpawn, RotationToSpawn);
	FTransform RoomTransform2 = SpawnedRoom2->GetFloorMesh()->GetSocketTransform(FName(RoomOpenings2[0]->SocketName), ERelativeTransformSpace::RTS_World);
	DrawDebugSphere(GetWorld(), RoomTransform2.GetLocation(), 20.f, 4, FColor::Green, true, -1.f, (uint8)0U, 1.f);
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

		GetNextXY(Opening->Orientation, PaperDungeon.Layout[X][Y], NextX, NextY);
		CreateNextTiles(PaperDungeon, PaperDungeon.Layout[X][Y], Opening, NextX, NextY);
	}

	PaperDungeon.Print();
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
		/*
		if (X < 0) { UE_LOG(LogTemp, Warning, TEXT("X < 0 (%i)"), X); }
		else if (Y < 0) { UE_LOG(LogTemp, Warning, TEXT("Y < 0 (%i)"), Y); }
		else if (X >= PaperDungeon.DungeonSize) { UE_LOG(LogTemp, Warning, TEXT("X >= PaperDungeon.DungeonSize (%i >= %i)"), X, PaperDungeon.DungeonSize); }
		else if (Y >= PaperDungeon.DungeonSize) { UE_LOG(LogTemp, Warning, TEXT("Y >= PaperDungeon.DungeonSize (%i >= %i)"), Y, PaperDungeon.DungeonSize); }
		else if (PaperDungeon.Layout[X][Y].IsOccupied) { UE_LOG(LogTemp, Warning, TEXT("PaperDungeon.Layout[X][Y].IsOccupied (%i, %i)"), X, Y); }
		*/

		// There is an Opening and we can't place another room beyond it. Place a Barrier instead
		FBarrier* Barrier = DungeonData->GetRandomBarrier();

		FRotator RotationToSpawn = GetSpawnRotation(PreviousOpening->Orientation, FRotator(0.f), PreviousRoom.Rotation);
		FVector LocationToSpawn = PreviousRoom.Location + GetPlacementOffset(PreviousRoom.Rotation, PreviousOpening->Location) * -1;
		PaperDungeon.AddBarrier(LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z, RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll, Barrier->RowName);
		return;
	}

	FRoom* CurrentRoom = DungeonData->GetRandomRoom();
	TArray<FOpening*> CurrentOpenings = DungeonData->GetOpeningsFor(CurrentRoom->RowName);
	int32 OpeningToUseIndex = FMath::Floor(FMath::RandRange(0, CurrentOpenings.Num() - 1));
	FRotator RotationToSpawn = GetSpawnRotation(PreviousOpening->Orientation, CurrentOpenings[OpeningToUseIndex]->Orientation, PreviousRoom.Rotation);
	FVector LocationToSpawn = PreviousRoom.Location + GetPlacementOffset(PreviousRoom.Rotation, PreviousOpening->Location) * -1 + GetPlacementOffset(RotationToSpawn, CurrentOpenings[OpeningToUseIndex]->Location);

	PaperDungeon.UpdateTile(X, Y, LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z, RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll, CurrentRoom->RowName);

	CurrentOpenings.RemoveAt(OpeningToUseIndex); // This is the Opening back to PreviousRoom; don't try to spawn a new Room here
	if (CurrentOpenings.Num() == 0) return; // There are no more Openings to select new Rooms for on CurrentRoom

	// Get and place new Rooms for all Openings off of CurrentRoom
	for (FOpening* Opening : CurrentOpenings)
	{
		int32 NextX = X;
		int32 NextY = Y;

		GetNextXY(Opening->Orientation, PaperDungeon.Layout[X][Y], NextX, NextY);
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
		//UE_LOG(LogTemp, Warning, TEXT("Offset: Yaw is 0"));
		return LocationToReturn;
	}
	else if (FMath::IsNearlyEqual(Rotation.Yaw, -270.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 90.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 450.f, 1.f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Offset: Yaw is 90"));
		LocationToReturn.X = Location.Y;
		LocationToReturn.Y = Location.X;
		return LocationToReturn * FVector(-1.f, 1.f, 1.f);
	}
	else if (FMath::IsNearlyEqual(Rotation.Yaw, 180.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 540.f, 1.f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Offset: Yaw is 180"));
		return LocationToReturn * FVector(-1.f, -1.f, 1.f);
	}
	else if (FMath::IsNearlyEqual(Rotation.Yaw, -90.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 270.f, 1.f) || FMath::IsNearlyEqual(Rotation.Yaw, 630.f, 1.f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Offset: Yaw is 270"));
		LocationToReturn.X = Location.Y;
		LocationToReturn.Y = Location.X;
		return LocationToReturn * FVector(1.f, -1.f, 1.f);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Offset: Yaw not found: %f"), Rotation.Yaw);
	}

	return FVector();
}

FRotator UDungeonBuilder::GetSpawnRotation(FRotator FirstRotation, FRotator SecondRotation, FRotator RoomRotation)
{
	FRotator OpeningOrientation = FirstRotation + RoomRotation;

	// Get the values between 0 and 360 so the math works well
	OpeningOrientation.Yaw = CorrectDegrees(OpeningOrientation.Yaw);

	//UE_LOG(LogTemp, Warning, TEXT("Opening Orientation 1: [%f, %f, %f]"), OpeningOrientation.Pitch, OpeningOrientation.Yaw, OpeningOrientation.Roll);
	//UE_LOG(LogTemp, Warning, TEXT("Opening Orientation 2: [%f, %f, %f]"), SecondOpening->Orientation.Pitch, SecondOpening->Orientation.Yaw, SecondOpening->Orientation.Roll);

	FRotator RotationToSpawn = SecondRotation - OpeningOrientation;
	if (FMath::IsNearlyEqual(OpeningOrientation.Yaw, SecondRotation.Yaw, 1.f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Yaws are equal; setting rotation to 180.f"));
		RotationToSpawn.Yaw = 180.f;
	}
	else if (FMath::IsNearlyEqual(FMath::Abs<float>(OpeningOrientation.Yaw - SecondRotation.Yaw), 180.f, 1.f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Yaws are 180.f apart; will set Rotation to 0.f"));
		RotationToSpawn.Yaw = 0.f;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Orientation to Rotate: [%f, %f, %f]"), RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll);
	return RotationToSpawn;
}

void UDungeonBuilder::GetNextXY(FRotator OpeningOrientation, FPaperDungeonTile& Tile, int32& NextX, int32& NextY)
{
	FRotator AbsoluteRotation = OpeningOrientation + Tile.Rotation;
	AbsoluteRotation.Yaw = CorrectDegrees(AbsoluteRotation.Yaw);

	if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 0.f, 1.f)) NextY -= 1;
	else if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 90.f, 1.f)) NextX -= 1;
	else if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 180.f, 1.f)) NextY += 1;
	else if (FMath::IsNearlyEqual(AbsoluteRotation.Yaw, 270.f, 1.f)) NextX += 1;
}

float UDungeonBuilder::CorrectDegrees(float DegreesToCorrect)
{
	float Yaw = DegreesToCorrect;

	while (Yaw > 360.f || FMath::IsNearlyEqual(Yaw, 360.f, 1.f))
	{
		Yaw -= 360.f;
	}
	if (Yaw < 0) Yaw += 360.f;

	return Yaw;
}
