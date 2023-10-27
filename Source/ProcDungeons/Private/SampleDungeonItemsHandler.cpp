// Fill out your copyright notice in the Description page of Project Settings.

#include "SampleDungeonItemsHandler.h"
#include "Engine/DataTable.h"
#include "Structs/SampleItem.h"

void USampleDungeonItemsHandler::HandleNewItem(FString Key, FVector Location, FRotator Rotation)
{
	UE_LOG(LogTemp, Warning, TEXT("USampleDungeonItemsHandler::HandleNewItem(): Received new item %s"), *Key);

	FSampleItem* Row = Items->FindRow<FSampleItem>(FName(Key), Context);
	GetWorld()->SpawnActor<AActor>(Row->bp_ref, Location, Rotation);
}
