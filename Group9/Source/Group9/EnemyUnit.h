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

	UPROPERTY(EditAnywhere)
	bool bDestroyMyselfOnDeath{ 0 };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleDestruction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class APlayerUnit* PlayerUnit;

	

	UPROPERTY(EditAnywhere)
	float MaxHealth{100};
	UPROPERTY(EditAnywhere)
	float CurrentHealth{100};

	bool bIsDead{ 0 };

	virtual void TakeDamage(float);
};
