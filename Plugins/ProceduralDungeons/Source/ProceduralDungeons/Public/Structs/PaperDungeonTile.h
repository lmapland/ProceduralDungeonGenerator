#pragma once

#include "CoreMinimal.h"
#include "PaperDungeonTile.generated.h"

USTRUCT(Blueprintable)
struct FPaperDungeonTile
{
	GENERATED_BODY()

	/* The chosen room to be spawned in this slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RoomName = FName("");

	/* The location for the Room to be spawned at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector(0.f, 0.f, 0.f);

	/* The rotation for the Room to be spawned at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	/* This is true if this tile spawns a Room (RoomName is set), but can also be true in the case that a neighboring Room
	   is more than 1x1x1 and so extends into this tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOccupied = false;
};