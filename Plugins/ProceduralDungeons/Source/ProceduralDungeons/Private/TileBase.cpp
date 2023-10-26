// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBase.h"

ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMeshComponent"));
	FloorMesh->SetupAttachment(RootComponent);
}
