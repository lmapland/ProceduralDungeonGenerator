#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/Room.h"
#include "Structs/Opening.h"
#include "Structs/Barrier.h"
#include "Structs/DungeonItem.h"
#include "DungeonData.generated.h"

class UDataTable;

/**
 *
 */
UCLASS()
class PROCEDURALDUNGEONS_API UDungeonData : public UObject
{
	GENERATED_BODY()

public:
	void Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers, UDataTable* InItems);
	FRoom* GetRoom(FName RoomName);
	FRoom* GetRandomRoom();
	FBarrier* GetBarrier(FName BarrierName);
	FBarrier* GetRandomBarrier();
	FDungeonItem* GetItem(FName ItemName);
	TArray<FOpening*> GetOpeningsFor(FName RoomName);

private:
	UDataTable* DTRooms;
	UDataTable* DTOpenings;
	UDataTable* DTBarriers;
	UDataTable* DTItems;

	TArray<FRoom*> Rooms;
	TArray<FBarrier*> Barriers;
	TArray<FDungeonItem*> Items;
	
	const FString Context = FString("");

public:
	FORCEINLINE int32 GetItemArraySize() { return Items.Num(); }
};
