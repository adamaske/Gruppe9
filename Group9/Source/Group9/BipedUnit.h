// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BipedUnit.generated.h"

UCLASS()
class GROUP9_API ABipedUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABipedUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bi Enemy")
		class AAIController* AIController;

	//Capsule collider for the coilliosn of the enemy
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		UCapsuleComponent* CapCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bi Enemy")
		class UStaticMeshComponent* BipedMesh;

	UPROPERTY(EditAnywhere)
		bool bDestroyMyselfOnDeath{ 0 };

	UPROPERTY(EditAnywhere)
		float MaxHealth{ 100 };
	UPROPERTY(EditAnywhere)
		float CurrentHealth{ 100 };

	bool bIsDead{ 0 };

	void TakeDamage(float);

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		class USphereComponent* EnemyFOVSphere;

	UFUNCTION()
		void SphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	//void MoveUnit(FVector LookAtTarget);

	void MoveToPlayer();*/

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Bi Enemy")
		float movementSpeed = 40;

	UPROPERTY(EditAnywhere, Category = "Bi Enemy")
		float BipedStopRange{ 150.f };

	//dash attack

	void DashMeleeAttack(float);


	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashRange{ 400.f };

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashSpeed{ 20.f };

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		class UBoxComponent* DashMeleeBox;

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashChargeTime{ 2 };

	float DashMovementEnd{ 0.4f };

	float DashHitTime{ 0 };

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float CurrentDashChargeTime{ 0 };

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashStart{ 0.f };

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashEnd{ 0.3f };

	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashDamage{ 50 };

	bool bIsCharging{ 0 };

	bool bDashhit{ 0 };

	bool bIsDashing{ 0 };

	UFUNCTION()
		void DashHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	//Close melee attack
	void CloseMeleeAttack(float);
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		class UBoxComponent* MeleeBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bi Melee")
		float MeleeDamage{ 25 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bi Melee")
		bool bIsAttacking{ 0 };

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay();

private:

	float PlayerDistance();

	FQuat Rotation;

	FTimerHandle MeleeTimerHandle;

	class APlayerUnit* PlayerUnit;

};
