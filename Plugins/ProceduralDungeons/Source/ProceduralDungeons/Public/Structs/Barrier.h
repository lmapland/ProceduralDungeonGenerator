#pragma once

#include "CoreMinimal.h"
#include "Barrier.generated.h"

USTRUCT(Blueprintable)
struct FBarrier : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> bp_ref;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDoor = false;

};