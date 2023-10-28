// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/DungeonBuilderTest.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonBuilder.h"
#include "DungeonData.h"
#include "DungeonItemsHandler.h"
#include "TileBase.h"

#include "DrawDebugHelpers.h"

void UDungeonBuilderTest::TestSouthOpening()
{
	ClearField();
	TestDungeonSuite(SouthRoom);
}

void UDungeonBuilderTest::TestWestOpening()
{
	ClearField();
	TestDungeonSuite(WestRoom);
}

void UDungeonBuilderTest::TestNorthOpening()
{
	ClearField();
	TestDungeonSuite(NorthRoom);
}

void UDungeonBuilderTest::TestEastOpening()
{
	ClearField();
	TestDungeonSuite(EastRoom);
}

void UDungeonBuilderTest::TestCorrectDegrees()
{
	UE_LOG(LogTemp, Display, TEXT("---TestCorrectDegrees---"));
	int32 TotalTests = 13;
	int32 PassingTests = TotalTests;

	UDungeonBuilder* Builder = CreateDungeonBuilder();
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("Builder object was empty. Exiting"));
		return;
	}

	// Base cases
	float TestValue = 0.f;
	{
		if (TestValue != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = 90.f;
		if (TestValue != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = 180.f;
		if (TestValue != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = 270.f;
		if (TestValue != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}
	}

	// Normal cases
	{
		TestValue = -270.f;
		if (90.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = -180.f;
		if (180.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = -90.f;
		if (270.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = 360.f;
		if (0.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = 450.f;
		if (90.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = 540.f;
		if (180.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}
	}

	// Bounds checking
	{
		TestValue = -0.000000001f;
		if (0.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = -360.f;
		if (0.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}

		TestValue = -359.99999f;
		if (0.f != Builder->CorrectDegrees(TestValue))
		{
			UE_LOG(LogTemp, Error, TEXT("TestCorrectDegrees: %f failed"), TestValue);
			PassingTests--;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("%i / %i Cases Pass"), PassingTests, TotalTests);
	UE_LOG(LogTemp, Display, TEXT("---End TestCorrectDegrees---"));
}

void UDungeonBuilderTest::TestGetNextXY()
{
	UE_LOG(LogTemp, Display, TEXT("---TestGetNextXY---"));
	int32 TotalTests = 25;
	int32 PassingTests = TotalTests;

	UDungeonBuilder* Builder = CreateDungeonBuilder();
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("Builder object was empty. Exiting"));
		return;
	}

	// Base cases
	// OpeningOrientation = 0.f
	FRotator OpeningOrientation = FRotator(0.f);
	FRotator RoomRotation = FRotator(0.f);
	int32 X = 0;
	int32 Y = 0;
	int32 ExpectedX = 0;
	int32 ExpectedY = -1;
	{
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = -1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 180.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: 0, -1 | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 270.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}
	}

	// OpeningOrientation = 90.f
	{
		OpeningOrientation = FRotator(90.f);
		RoomRotation = FRotator(0.f);
		X = 0;
		Y = 0;
		ExpectedX = -1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 180.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: 0, -1 | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 270.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = -1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}
	}

	// OpeningOrientation = 180.f
	{
		OpeningOrientation = FRotator(180.f);
		RoomRotation = FRotator(0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 180.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = -1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: 0, -1 | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 270.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = -1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}
	}

	// OpeningOrientation = 270.f
	{
		OpeningOrientation = FRotator(270.f);
		RoomRotation = FRotator(0.f);
		X = 0;
		Y = 0;
		ExpectedX = 1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = -1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 180.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = -1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: 0, -1 | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 270.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}
	}

	// -90 tests
	{
		OpeningOrientation = FRotator(0.f);
		RoomRotation = FRotator(0.f, -90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		OpeningOrientation = FRotator(90.f);
		RoomRotation = FRotator(0.f, -90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = -1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		OpeningOrientation = FRotator(180.f);
		RoomRotation = FRotator(0.f, -90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = -1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		OpeningOrientation = FRotator(-90.f);
		RoomRotation = FRotator(0.f);
		X = 0;
		Y = 0;
		ExpectedX = 1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = -1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		RoomRotation = FRotator(0.f, 180.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = -1;
		ExpectedY = 0;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: 0, -1 | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		OpeningOrientation = FRotator(0.f, 270.f, 0.f);
		RoomRotation = FRotator(0.f, -90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		OpeningOrientation = FRotator(0.f, -90.f, 0.f);
		RoomRotation = FRotator(0.f, 270.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}

		OpeningOrientation = FRotator(0.f, -90.f, 0.f);
		RoomRotation = FRotator(0.f, -90.f, 0.f);
		X = 0;
		Y = 0;
		ExpectedX = 0;
		ExpectedY = 1;
		Builder->GetNextXY(OpeningOrientation, RoomRotation, X, Y);
		if (X != ExpectedX || Y != ExpectedY)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & %f: Expected: %i, %i | Actual: %i, %i"), OpeningOrientation.Yaw, RoomRotation.Yaw, ExpectedX, ExpectedY, X, Y);
			PassingTests--;
		}
	}


	UE_LOG(LogTemp, Display, TEXT("%i / %i Cases Pass"), PassingTests, TotalTests);
	UE_LOG(LogTemp, Display, TEXT("---End TestGetNextXY---"));
}

void UDungeonBuilderTest::TestGetSpawnRotation()
{
	UE_LOG(LogTemp, Display, TEXT("---TestGetSpawnRotation---"));
	int32 TotalTests = 16;
	int32 PassingTests = TotalTests;

	UDungeonBuilder* Builder = CreateDungeonBuilder();
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("Builder object was empty. Exiting"));
		return;
	}

	// Although these should be testing all/most possible inputs
	// I happen to know that the FirstRotation & RoomRotation are summed together and then CorrectDegrees is called on the result
	// So I am assuming that TestCorrectDegrees() will find any issues with that function and I can therefore simplify my inputs
	// and fully test (the sum of FirstRotation and RoomRotation) with SecondRotation.
	// So that's why my tests are they way they are here.

	// Base cases
	// RoomRotation = 0.f
	FRotator FirstRotation = FRotator(0.f);
	FRotator SecondRotation = FRotator(0.f);
	FRotator RoomRotation = FRotator(0.f);
	FRotator Expected = FRotator(0.f, 180.f, 0.f);
	FRotator Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
	{
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(90.f);
		Expected = FRotator(0.f, 90.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(180.f);
		Expected = FRotator(0.f, 0.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(270.f);
		Expected = FRotator(0.f, 270.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}
	}

	// RoomRotation = 90.f
	{
		SecondRotation = FRotator(0.f);
		RoomRotation = FRotator(90.f);
		Expected = FRotator(0.f, 270.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(90.f);
		Expected = FRotator(0.f, 180.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(180.f);
		Expected = FRotator(0.f, 90.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(270.f);
		Expected = FRotator(0.f, 0.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}
	}

	// RoomRotation = 180.f
	{
		SecondRotation = FRotator(0.f);
		RoomRotation = FRotator(180.f);
		Expected = FRotator(0.f, 0.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(90.f);
		Expected = FRotator(0.f, 270.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(180.f);
		Expected = FRotator(0.f, 180.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(270.f);
		Expected = FRotator(0.f, 90.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}
	}

	// RoomRotation = 270.f
	{
		SecondRotation = FRotator(0.f);
		RoomRotation = FRotator(270.f);
		Expected = FRotator(0.f, 90.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(90.f);
		Expected = FRotator(0.f, 0.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(180.f);
		Expected = FRotator(0.f, 270.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}

		SecondRotation = FRotator(270.f);
		Expected = FRotator(0.f, 180.f, 0.f);
		Actual = Builder->GetSpawnRotation(FirstRotation, SecondRotation, RoomRotation);
		if (!FMath::IsNearlyEqual(Expected.Yaw, Actual.Yaw, 1.f))
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f & %f: Expected: %f | Actual: %f"), FirstRotation.Yaw, SecondRotation.Yaw, RoomRotation.Yaw, Expected.Yaw, Actual.Yaw);
			PassingTests--;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("%i / %i Cases Pass"), PassingTests, TotalTests);
	UE_LOG(LogTemp, Display, TEXT("---End TestGetSpawnRotation---"));
}

void UDungeonBuilderTest::TestGetPlacementOffset()
{
	UE_LOG(LogTemp, Display, TEXT("---TestGetPlacementOffset---"));
	int32 TotalTests = 8;
	int32 PassingTests = TotalTests;

	UDungeonBuilder* Builder = CreateDungeonBuilder();
	if (!Builder)
	{
		UE_LOG(LogTemp, Error, TEXT("Builder object was empty. Exiting"));
		return;
	}

	// positives
	FRotator Rotation = FRotator(0.f);
	FVector Location = FVector(500.f, 300.f, 0.f);
	FVector Expected = Location;
	FVector Actual = Builder->GetPlacementOffset(Rotation, Location);
	{
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}

		Rotation = FRotator(90.f);
		Expected = FVector(-300.f, 500.f, 0.f);
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}

		Rotation = FRotator(180.f);
		Expected = FVector(-500.f, -300.f, 0.f);
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}

		Rotation = FRotator(270.f);
		Expected = FVector(300.f, -500.f, 0.f);
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}
	}

	// negatives
	{
		Rotation = FRotator(0.f);
		Location = FVector(-400.f, -200.f, 0.f);
		Expected = Location;
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}

		Rotation = FRotator(90.f);
		Expected = FVector(200.f, -400.f, 0.f);
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}

		Rotation = FRotator(180.f);
		Expected = FVector(400.f, 200.f, 0.f);
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}

		Rotation = FRotator(270.f);
		Expected = FVector(-200.f, 400.f, 0.f);
		Actual = Builder->GetPlacementOffset(Rotation, Location);
		if (Expected != Actual)
		{
			UE_LOG(LogTemp, Error, TEXT("%f & (%f, %f): Expected: %f,%f | Actual: %f,%f"), Rotation.Yaw, Location.X, Location.Y, Expected.X, Expected.Y, Actual.X, Actual.Y);
			PassingTests--;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("%i / %i Cases Pass"), PassingTests, TotalTests);
	UE_LOG(LogTemp, Display, TEXT("---End TestGetPlacementOffset---"));
}

UDungeonBuilder* UDungeonBuilderTest::CreateDungeonBuilder()
{
	UDungeonItemsHandler* ItemsHandler = NewObject<UDungeonItemsHandler>();
	UDungeonBuilder* Builder = NewObject<UDungeonBuilder>();
	Builder->Setup(Rooms, Openings, Barriers, Items, ItemsHandler);

	return Builder;
}

void UDungeonBuilderTest::TestDungeonSuite(FName RoomToSpawn)
{
	FVector Start = FVector(0.f);
	FVector OffsetX = FVector(2500.f, 0.f, 0.f);
	FVector OffsetY = FVector(0.f, 3000.f, 0.f);

	NewTestDungeon(RoomToSpawn, 0, 0.f, Start);
	NewTestDungeon(RoomToSpawn, 1, 0.f, Start + OffsetX);
	NewTestDungeon(RoomToSpawn, 2, 0.f, Start + OffsetX * 2.f);
	NewTestDungeon(RoomToSpawn, 3, 0.f, Start + OffsetX * 3.f);

	NewTestDungeon(RoomToSpawn, 0, 90.f, Start + OffsetY);
	NewTestDungeon(RoomToSpawn, 1, 90.f, Start + OffsetY + OffsetX);
	NewTestDungeon(RoomToSpawn, 2, 90.f, Start + OffsetY + OffsetX * 2.f);
	NewTestDungeon(RoomToSpawn, 3, 90.f, Start + OffsetY + OffsetX * 3.f);

	NewTestDungeon(RoomToSpawn, 0, 180.f, Start + OffsetY * 2);
	NewTestDungeon(RoomToSpawn, 1, 180.f, Start + OffsetY * 2 + OffsetX);
	NewTestDungeon(RoomToSpawn, 2, 180.f, Start + OffsetY * 2 + OffsetX * 2.f);
	NewTestDungeon(RoomToSpawn, 3, 180.f, Start + OffsetY * 2 + OffsetX * 3.f);

	NewTestDungeon(RoomToSpawn, 0, 270.f, Start + OffsetY * 3);
	NewTestDungeon(RoomToSpawn, 1, 270.f, Start + OffsetY * 3 + OffsetX);
	NewTestDungeon(RoomToSpawn, 2, 270.f, Start + OffsetY * 3 + OffsetX * 2.f);
	NewTestDungeon(RoomToSpawn, 3, 270.f, Start + OffsetY * 3 + OffsetX * 3.f);
}

void UDungeonBuilderTest::NewTestDungeon(FName RoomToSpawn, int32 Socket, float Yaw, FVector StartingLocation)
{
	UDungeonBuilder* Builder = CreateDungeonBuilder();

	FRoom* Room = Builder->DungeonData->GetRoom(StartRoom);

	// Place down the very first room - the Start Piece
	FRotator InitialRotation(0.f, Yaw, 0.f);
	ATileBase* SpawnedRoom = GetWorld()->SpawnActor<ATileBase>(Room->bp_ref, StartingLocation, InitialRotation);
	TArray<FOpening*> RoomOpenings = Builder->DungeonData->GetOpeningsFor(StartRoom);

	FRoom* Room2 = Builder->DungeonData->GetRoom(RoomToSpawn);
	TArray<FOpening*> RoomOpenings2 = Builder->DungeonData->GetOpeningsFor(Room2->RowName);
	if (RoomOpenings2.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomOpenings2.Num() is empty!"));
		return;
	}

	FVector RoomLocation = SpawnedRoom->GetActorLocation();
	FVector SocketOffset = Builder->GetPlacementOffset(InitialRotation, RoomOpenings[Socket]->Location) * -1;

	UE_LOG(LogTemp, Display, TEXT("--- Socket: %i, Yaw: %f ---"), Socket, Yaw);
	UE_LOG(LogTemp, Display, TEXT("Room1 Orientation: [%f, %f, %f]"), SpawnedRoom->GetActorRotation().Pitch, SpawnedRoom->GetActorRotation().Yaw, SpawnedRoom->GetActorRotation().Roll);

	FRotator RotationToSpawn = Builder->GetSpawnRotation(RoomOpenings[Socket]->Orientation, RoomOpenings2[0]->Orientation, SpawnedRoom->GetActorRotation());
	DrawDebugSphere(GetWorld(), RoomLocation + SocketOffset, 20.f, 4, FColor::Red, true, -1.f, (uint8)0U, 1.f);
	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %f + %f = %f"), RoomOpenings[Socket]->Orientation.Yaw, RoomOpenings2[0]->Orientation.Yaw, RotationToSpawn.Yaw);

	//UE_LOG(LogTemp, Warning, TEXT("Opening1 Location: [%f, %f, %f]"), (RoomLocation + SocketOffset).X, (RoomLocation + SocketOffset).Y, (RoomLocation + SocketOffset).Z);
	FVector LocationOffset = Builder->GetPlacementOffset(RotationToSpawn, RoomOpenings2[0]->Location);
	//UE_LOG(LogTemp, Warning, TEXT("LocationOffset: [%f, %f, %f]"), LocationOffset.X, LocationOffset.Y, LocationOffset.Z);

	FVector LocationToSpawn = RoomLocation + SocketOffset + LocationOffset;
	//UE_LOG(LogTemp, Warning, TEXT("Final Location: [%f, %f, %f]"), LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z);

	ATileBase* SpawnedRoom2 = GetWorld()->SpawnActor<ATileBase>(Room2->bp_ref, LocationToSpawn, RotationToSpawn);
	FTransform RoomTransform2 = SpawnedRoom2->GetFloorMesh()->GetSocketTransform(FName(RoomOpenings2[0]->SocketName), ERelativeTransformSpace::RTS_World);
	DrawDebugSphere(GetWorld(), RoomTransform2.GetLocation(), 20.f, 4, FColor::Green, true, -1.f, (uint8)0U, 1.f);

	// spawn items in Room2
	/*for (FString ItemName : Room2->ItemNames)
	{
		FDungeonItem* Item = DungeonData->GetItem(FName(ItemName));
		if (Item == nullptr) continue;

		float ToSpawn = FMath::RandRange(0.f, 1.f);
		if (ToSpawn > Item->SpawnLikelihood) continue; // Don't spawn the item

		FRotator Rotation = RotationToSpawn + Item->Rotation;
		Rotation.Yaw = CorrectDegrees(Rotation.Yaw);
		//UE_LOG(LogTemp, Warning, TEXT("Room Rotation: [%f, %f, %f]"), RotationToSpawn.Pitch, RotationToSpawn.Yaw, RotationToSpawn.Roll);
		//UE_LOG(LogTemp, Warning, TEXT("Item Rotation: [%f, %f, %f]"), Item->Rotation.Pitch, Item->Rotation.Yaw, Item->Rotation.Roll);
		//UE_LOG(LogTemp, Warning, TEXT("Rotation: %f + %f = %f"), RotationToSpawn.Yaw, Item->Rotation.Yaw, Rotation.Yaw);

		//UE_LOG(LogTemp, Warning, TEXT("Initial Item Location X,Y: [%f, %f]"), Item->Location.X, Item->Location.Y);
		FVector ItemLocation = GetPlacementOffset(RotationToSpawn, Item->Location);
		FVector Location = LocationToSpawn + ItemLocation;
		//UE_LOG(LogTemp, Warning, TEXT("Room Location: [%f, %f, %f]"), LocationToSpawn.X, LocationToSpawn.Y, LocationToSpawn.Z);
		//UE_LOG(LogTemp, Warning, TEXT("Item Location: [%f, %f, %f]"), ItemLocation.X, ItemLocation.Y, ItemLocation.Z);
		//UE_LOG(LogTemp, Warning, TEXT("Location: %f + %f = %f"), LocationToSpawn.Y, ItemLocation.Y, Location.Y);

		ItemsHandler->HandleNewItem(Item->Key, Location, Rotation);
	}*/
}

void UDungeonBuilderTest::ClearField()
{
	TArray<AActor*> TileActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATileBase::StaticClass(), TileActors);
	for (AActor* Tile : TileActors)
	{
		Tile->Destroy();
	}

}
