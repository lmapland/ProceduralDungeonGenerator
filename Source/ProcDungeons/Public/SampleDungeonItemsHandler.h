// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonItemsHandler.h"
#include "SampleDungeonItemsHandler.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS(Blueprintable)
class PROCDUNGEONS_API USampleDungeonItemsHandler : public UDungeonItemsHandler
{
	GENERATED_BODY()
	
public:
	virtual void HandleNewItem(FString Key, FVector Location, FRotator Rotation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Items;

private:
	FString Context = FString("");
};
