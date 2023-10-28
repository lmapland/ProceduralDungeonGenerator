// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonBuilderTest.generated.h"

class UDataTable;
class UDungeonBuilder;

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDURALDUNGEONS_API UDungeonBuilderTest : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TestSouthOpening();

	UFUNCTION(BlueprintCallable)
	void TestWestOpening();

	UFUNCTION(BlueprintCallable)
	void TestNorthOpening();

	UFUNCTION(BlueprintCallable)
	void TestEastOpening();

	UFUNCTION(BlueprintCallable)
	void TestCorrectDegrees();

	UFUNCTION(BlueprintCallable)
	void TestGetNextXY();

	UFUNCTION(BlueprintCallable)
	void TestGetSpawnRotation();

	UFUNCTION(BlueprintCallable)
	void TestGetPlacementOffset();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Rooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Openings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Barriers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Items;
	
	// If your log level is below 3 you may not get the output from the tests you expect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LogLevel = 3;

private:
	UDungeonBuilder* CreateDungeonBuilder();
	void TestDungeonSuite(FName RoomToSpawn);
	void NewTestDungeon(FName RoomToSpawn, int32 Socket, float Yaw, FVector StartingLocation);
	void ClearField();

	UPROPERTY(VisibleAnywhere)
	FName SouthRoom = FName("Room1");
	
	UPROPERTY(VisibleAnywhere)
	FName WestRoom = FName("Room6");
	
	UPROPERTY(VisibleAnywhere)
	FName NorthRoom = FName("Room7");
	
	UPROPERTY(VisibleAnywhere)
	FName EastRoom = FName("Room8");

	UPROPERTY(VisibleAnywhere)
	FName StartRoom = FName("Room5");

};
