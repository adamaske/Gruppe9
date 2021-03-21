// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class GROUP9_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class APlayerUnit* PlayerUnit;

	void DoSpawning();

	UPROPERTY(EditAnywhere)
		float Cooldown{ 5 };
	float currentTime{ 0 };
	UPROPERTY(EditAnywhere)
		float MaxEnemiesCount{ 10 };
	UPROPERTY(VisibleAnywhere)
		float CurrentEnemiesCount{ 0 };
	UPROPERTY(EditAnywhere)
		float MaxDistanceFromPlayerToSpawn{ 400 };
	UPROPERTY(EditAnywhere)
		float MinDistanceFromPlayerToSpawn{ 100 };

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyUnit> EnemyBlueprint;
	TArray<AEnemyUnit*> AliveEnemies;

	TArray<AActor*> tempArray;
	UPROPERTY(EditAnywhere)
		TArray<class ARoom*> Rooms;

	UPROPERTY(EditAnywhere)
		TArray<class ADoor*> Doors;

};
