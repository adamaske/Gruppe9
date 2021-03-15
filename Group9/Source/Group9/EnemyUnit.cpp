// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUnit.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyUnit::AEnemyUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;





	
}

// Called when the game starts or when spawned
void AEnemyUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

