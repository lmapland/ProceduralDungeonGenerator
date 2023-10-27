// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonItemsHandler.generated.h"

/**
 * 
 UDungeonItemsHandler
 * 
 * Used by the DungeonGenerator
 * HandleNewItem is called by the DungeonGenerator when it determines that a new Item should be created
 * The developer should subclass this class and fill out HandleNewItem() to do the appropriate thing for their item system
 * 
 */
UCLASS()
class PROCEDURALDUNGEONS_API UDungeonItemsHandler : public UObject
{
	GENERATED_BODY()

public:
	virtual void HandleNewItem(FString Key, FVector Location, FRotator Rotation);
	
};
