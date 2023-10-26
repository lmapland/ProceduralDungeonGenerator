#pragma once

#include "CoreMinimal.h"
#include "Room.generated.h"

USTRUCT(Blueprintable)
struct FRoom : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UStaticMesh> mesh_ref;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> bp_ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Size = FVector(1.f, 1.f, 1.f);

	// Contains unique ids for the FOpening DataTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Openings;

	// True if Openings array is empty
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEndPiece = false;

};