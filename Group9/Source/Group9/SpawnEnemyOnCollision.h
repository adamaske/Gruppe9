// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnEnemyOnCollision.generated.h"

UCLASS()
class GROUP9_API ASpawnEnemyOnCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnEnemyOnCollision();
	
	bool bHasSpawned{ 0 };

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyUnit> EnemyBP{nullptr};

	UPROPERTY(EditAnywhere)
		class USphereComponent* MyCollisionComponent;
	UPROPERTY(EditAnywhere)
		class USceneComponent* EnemySpawnPoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn(class APlayerUnit*);
	UFUNCTION()
		void BeganOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndedOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
