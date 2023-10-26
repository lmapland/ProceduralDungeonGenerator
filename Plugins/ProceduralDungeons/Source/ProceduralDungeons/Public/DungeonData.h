#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/Room.h"
#include "Structs/Opening.h"
#include "Structs/Barrier.h"
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
	void Setup(UDataTable* InRooms, UDataTable* InOpenings, UDataTable* InBarriers);
	FRoom* GetRoom(FName RoomName);
	FRoom* GetRandomRoom();
	FBarrier* GetBarrier(FName BarrierName);
	FBarrier* GetRandomBarrier();
	TArray<FOpening*> GetOpeningsFor(FName RoomName);

private:
	UDataTable* DTRooms;
	UDataTable* DTOpenings;
	UDataTable* DTBarriers;

	TArray<FRoom*> Rooms;
	TArray<FBarrier*> Barriers;
	
	const FString Context = FString("");
};
