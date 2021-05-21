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
	//MESH for Biped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bi Enemy")
		class UStaticMeshComponent* BipedMesh;
	//stop range to make the character stop while meleeing
	UPROPERTY(EditAnywhere, Category = "Bi Enemy")
		float BipedStopRange{ 150.f };
	//destroys the unit on death 
	UPROPERTY(EditAnywhere)
		bool bDestroyMyselfOnDeath{ 0 };
	//max health
	UPROPERTY(EditAnywhere)
		float MaxHealth{ 100 };
	UPROPERTY(EditAnywhere)
		float CurrentHealth{ 100 };
	//Bool for is dead to remove the character
	bool bIsDead{ 0 };
	//take damage function from enenmy unit
	void TakeDamage(float);
	//bool for dashing, makes it so that enemeies cant dash every tick
	bool HasDashed{ 0 };

	//Animations bools for BP

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation for Biped")
		bool AnimIsAttacking{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation for Biped")
		bool AnimIsCharging{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation for Biped")
		bool AnimIsWalking{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation for Biped")
		bool AnimIsDashing{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation for Biped")
		bool AnimDasAttacking{ 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation for Biped")
		bool AnimDeath{ 0 };


	//dash attack
	void DashMeleeAttack(float);

	//Dash range for checking when to dash
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashRange{ 400.f };
	//dash speed when dashing
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashSpeed{ 1500.f };
	//Dash collider for overlap events
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		class UBoxComponent* DashMeleeBox;
	//how much time the character waits before dashing
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashChargeTime{ 2 };
	//the amount of time the unit dashes
	float DashMovementEnd{ 0.4f };
	//time for managing overlap events
	float DashHitTime{ 0 };
	//current dash time for checking how long the unit waits to charge
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float CurrentDashChargeTime{ 0 };
	//time for marking start dash charge
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashStart{ 0.f };
	//time for marking dash end
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashEnd{ 0.3f };
	//dash damage amount
	UPROPERTY(EditAnywhere, Category = "Bi Dash")
		float DashDamage{ 50 };
	//bool for check if charging
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	bool bIsCharging{ 0 };
	//bool for checking of dash has hit to avoid damage stacking
	bool bDashhit{ 0 };
	//bool for dashing to avoid multible move events happeining at the same time
	bool bIsDashing{ 0 };
	//overlap event for dash attack
	UFUNCTION()
		void DashHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	//Close melee attack
	void CloseMeleeAttack(float);
	//collider for normal melee attack
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		class UBoxComponent* MeleeBox;
	//melee damage 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bi Melee")
		float MeleeDamage{ 25 };
	//bool to avoid damage stacking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bi Melee")
		bool bIsAttacking{ 0 };
	//time manager eneabling/disabling overlap events for normal melee
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float CurrentTime{ 0 };
	//max time for the melee cycle overlap events
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float MeleeHitTime{ 1 };
	//where in currentTime the coverlap box eneables
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float MeleeStart{ 0.65f };
	//where it ends
	UPROPERTY(EditAnywhere, Category = "Bi Melee")
		float MeleeEnd{ 0.85f };
	//bool to avoid damage stacking
	bool bMeleeHit{ 0 };
	//overlap event function
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

	FTimerHandle MeleeTimerHandle;
	//pointer to player for damage and navigation
	class APlayerUnit* PlayerUnit;

};
