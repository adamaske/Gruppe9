// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerUnit.generated.h"

UCLASS()
class GROUP9_API APlayerUnit : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraBoom;
public:
	// Sets default values for this character's properties
	APlayerUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Interacting with Interactables
	void Interact();
	UPROPERTY(VisibleAnywhere, Category = "Interacting")
		class AInteractableUnit* CurrentInteractableUnit{ nullptr };
	void GetInteractableUnit(AInteractableUnit* unit);
	void RemoveInteractableUnit(AInteractableUnit* unit);

	//Spawn points
	void GetSpawnPointStation(class ASavePointStation* station);
	class ASavePointStation* CurrentSavePointStation{ nullptr };

	//Movement
	void MoveForward(float);
	void MoveRight(float);
	FVector MovementVector{ 0.f,0.f,0.f };
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MovementSpeed{ 400 };
	UPROPERTY(EditAnywhere, Category = "Stats")
		bool bCameraForward{ 1 };
	APlayerController* PC{nullptr};

	//Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxHealth{ 100 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CurrentHealth{ 100 };

	void TakeDamage(float damage);

	void TakeDamageTest();
	//Combat
	void Shoot();
	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireRate{ 0.1 };
	float ShootingTimer{ 0 };
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class ABullet> BulletBlueprint{nullptr};
	//Ammunition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float CurrentAmmunition{0};
	void GetAmmunition(float);

	//Rooms
	void GetRoom(class ARoom*);

	class ARoom* CurrentRoom{ nullptr };
};
