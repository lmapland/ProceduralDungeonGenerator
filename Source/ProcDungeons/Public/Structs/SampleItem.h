#pragma once

#include "CoreMinimal.h"
#include "SampleItem.generated.h"

USTRUCT(Blueprintable)
struct FSampleItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> bp_ref;
};