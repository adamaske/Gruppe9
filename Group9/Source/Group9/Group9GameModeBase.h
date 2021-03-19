// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Group9GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AGroup9GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		AGroup9GameModeBase();
public:
	TSubclassOf<class APlayerUnit> PlayerBlueprint;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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
