// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "PlayerUnit.h"
#include "Kismet/GameplayStatics.h"
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

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		float movementSpeed = 40;

	UPROPERTY(EditAnywhere, Category = "Bi Enemy")
		float BipedStopRange{ 100.f };

	void CloseMeleeAttack(float);
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		class UBoxComponent* MeleeBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bi Melee")
		float MeleeDamage{ 50 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bi Melee")
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float CurrentTime{ 0 };

	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float MeleeHitTime{ 1 };

	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float MeleeStart{ 0.65f };

	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float MeleeEnd{ 0.85f };

	bool bMeleeHit{ 0 };

	UFUNCTION()
		void MeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay();

private:

	float PlayerDistance();

	FQuat Rotation;

	FTimerHandle MeleeTimerHandle;

	APlayerUnit* PlayerUnit;


};
