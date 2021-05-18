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

	UPROPERTY(EditAnywhere, Category = "Player")
		class APlayerUnit* PlayerUnit{ nullptr };
	UPROPERTY(EditAnywhere, Category = "Player")
	float KillPlayerZValue{-300};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving")
		FString SaveFileName { "" };
	void CheckPlayer();

	void DoSpawning();

	void CheckEnemies();
	UPROPERTY(EditAnywhere)
		float Cooldown{ 5 };
	UPROPERTY(EditAnywhere)
		bool bDoSpawning{0};
	float currentTime{ 0 };
	UPROPERTY(EditAnywhere, Category = "Enemies")
		float MaxEnemiesCount{ 10 };
	UPROPERTY(VisibleAnywhere, Category = "Enemies")
		float CurrentEnemiesCount{ 0 };
	UPROPERTY(EditAnywhere, Category = "Enemies")
		float MaxDistanceFromPlayerToSpawn{ 400 };
	UPROPERTY(EditAnywhere, Category = "Enemies")
		float MinDistanceFromPlayerToSpawn{ 100 };

	UPROPERTY(EditAnywhere, Category = "Enemies")
		TSubclassOf<class AEnemyUnit> EnemyBlueprint;
	TArray<AEnemyUnit*> AliveEnemies;

	UPROPERTY(EditAnywhere, Category = "Rooms")
		bool bFindDoorsAndRoomsOnBeginPlay{ 1 };
	TArray<AActor*> tempArray;
	UPROPERTY(EditAnywhere, Category = "Rooms")
		TArray<class ARoom*> Rooms;

	UPROPERTY(EditAnywhere, Category = "Rooms")
		TArray<class ADoor*> Doors;

	UPROPERTY(EditAnywhere, Category = "Rooms")
		TArray<class ASavePointStation*> SaveStations;

	UPROPERTY(EditAnywhere)
		bool bLoadOnBeginPlay{ 1 };

	UPROPERTY(EditAnywhere)
		bool bLoadOnPlayerDeath{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		float DeathTimer{ 2.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Player")
		float CurrentDeathTimer{ 0.f };
	bool bPlayerIsDead{ 0 };

	void PlayerDead();
	UFUNCTION(BlueprintCallable)
	void SaveTheGame();
	UFUNCTION(BlueprintCallable)
	void LoadTheGame();
	
	//Drop on enemy deaths
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AInteractableUnit>> PossibleDrops;
	void DoDrop(FVector location);
	UPROPERTY(EditAnywhere)
		float chanceToDrop{ 0.5 };
};
