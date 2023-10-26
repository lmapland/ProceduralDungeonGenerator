// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.generated.h"

UCLASS()
class PROCEDURALDUNGEONS_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATileBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FloorMesh;

public:
	FORCEINLINE UStaticMeshComponent* GetFloorMesh() { return FloorMesh; }
};
