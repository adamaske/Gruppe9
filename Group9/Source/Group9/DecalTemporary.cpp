// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalTemporary.h"

// Sets default values
ADecalTemporary::ADecalTemporary()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADecalTemporary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalTemporary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADecalTemporary::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

