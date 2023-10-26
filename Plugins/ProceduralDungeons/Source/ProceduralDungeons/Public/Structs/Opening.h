#pragma once

#include "CoreMinimal.h"
#include "Opening.generated.h"

USTRUCT(Blueprintable)
struct FOpening : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SocketName;
	
	// Location relative to the mesh's origin
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Orientation = FRotator(0.f, 0.f, 0.f);
};