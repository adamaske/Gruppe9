// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyUnit.generated.h"

class UCapsuleComponent;

UCLASS()
class GROUP9_API AEnemyUnit : public AActor
{
	GENERATED_BODY()

private:
	


public:	
	// Sets default values for this actor's properties
	AEnemyUnit();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleDestruction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Components")
	USceneComponent* BulletSpawnPoint;

	UPROPERTY(EditAnywhere)
	float MaxHealth{100};
	UPROPERTY(EditAnywhere)
	float CurrentHealth{100};

	bool bIsDead{ 0 };

	virtual void TakeDamage(float);
	//UPROPERTY(EditAnywhere, Category = "Enemy Stats")
		//float  EnemyMovementSpeed{ 300 };

	


	

};
