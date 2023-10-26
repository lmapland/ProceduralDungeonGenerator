// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonData.h"
#include "Engine/DataTable.h"

void UDungeonData::Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers)
{
	DTRooms = InRooms;
	DTOpenings = InOpenings;
	DTBarriers = InBarriers;

	DTRooms->GetAllRows<FRoom>(Context, Rooms);
	DTBarriers->GetAllRows<FBarrier>(Context, Barriers);
}

FRoom* UDungeonData::GetRoom(FName RoomName)
{
	return DTRooms->FindRow<FRoom>(RoomName, Context);
}

FRoom* UDungeonData::GetRandomRoom()
{
	int32 Rand = FMath::Floor(FMath::RandRange(0, Rooms.Num() - 1));

	return Rooms[Rand];
}

FBarrier* UDungeonData::GetBarrier(FName BarrierName)
{
	return DTBarriers->FindRow<FBarrier>(BarrierName, Context);
}

FBarrier* UDungeonData::GetRandomBarrier()
{
	int32 Rand = FMath::Floor(FMath::RandRange(0, Barriers.Num() - 1));

	return Barriers[Rand];
}

TArray<FOpening*> UDungeonData::GetOpeningsFor(FName RoomName)
{
	FRoom* RoomRow = DTRooms->FindRow<FRoom>(RoomName, Context);
	TArray<FOpening*> OpeningsForRoom;

	if (RoomRow == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOpeningsFor(): No RoomRow was found for room name: %s"), *RoomName.ToString());
		return OpeningsForRoom;
	}

	for (int32 i = 0; i < RoomRow->Openings.Num(); i++)
	{
		OpeningsForRoom.Add(DTOpenings->FindRow<FOpening>((FName(RoomRow->Openings[i])), Context));
	}

	return OpeningsForRoom;
}
