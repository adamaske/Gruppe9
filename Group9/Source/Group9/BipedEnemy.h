// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "PlayerUnit.h"
#include "BipedEnemy.generated.h"


UCLASS()
class GROUP9_API ABipedEnemy : public AEnemyUnit
{
	GENERATED_BODY()


public:

	ABipedEnemy();

	//Capsule collider for the coilliosn of the enemy
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		UCapsuleComponent* CapCollider;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bi Enemy")
		UStaticMeshComponent* BipedMesh;

	void MoveUnit(FVector LookAtTarget);


	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		float movementSpeed = 40;

	

	UPROPERTY(EditAnywhere, Category = "Bi Enemy")
		float BipedStopRange{ 100.f };

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay();

private:

	float PlayerDistance();

	FQuat Rotation;

	FTimerHandle MeleeTimerHandle;

	APlayerUnit* PlayerUnit;


};
