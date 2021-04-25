// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUnit.h"
#include "Components/CapsuleComponent.h"
#include "PlayerUnit.h"
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

void AEnemyUnit::HandleDestruction()
{

}

// Called every frame
void AEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyUnit::TakeDamage(float dmg) {
	UE_LOG(LogTemp, Log, TEXT("Enemy took damage"));
	CurrentHealth -= dmg;
	if (CurrentHealth <= 0) {
		//Die
		UE_LOG(LogTemp, Log, TEXT("Enemy dead"));
		bIsDead = true;

		if (bIsDead && bDestroyMyselfOnDeath) {
			this->Destroy();
		}
	}
}