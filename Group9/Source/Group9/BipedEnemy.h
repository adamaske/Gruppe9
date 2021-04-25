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

	//a sphere that works as a primitive line of sight
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		USphereComponent* SphereTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bi Enemy")
		UStaticMeshComponent* BipedMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		AAIController* AIController;

	//move to player unit func
	void MoveToTarget(APlayerUnit* PlayerUnit1);

	// function to active when playe enters NPC area
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	//function that activtaes on player leaving NPC area
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	// sets up movement for enemy unit
	virtual void  SetupPlayerInputComponent(UInputComponent* InputComponent);

protected:

	virtual void BeginPlay(); 

public:


};
