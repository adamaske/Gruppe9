// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "TestEnemyUnit.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API ATestEnemyUnit : public AEnemyUnit
{
	GENERATED_BODY()

		ATestEnemyUnit();
public:
	UStaticMeshComponent* TestMesh;

	void Movement(float);
	float movementSpeed = 400;
	virtual void Tick(float) override;
	
};
