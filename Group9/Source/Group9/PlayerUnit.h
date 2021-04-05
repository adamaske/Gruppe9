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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interacting")
		class AInteractableUnit* CurrentInteractableUnit{ nullptr };
	void GetInteractableUnit(AInteractableUnit* unit);
	void RemoveInteractableUnit(AInteractableUnit* unit);
	UPROPERTY(BlueprintReadOnly)
	bool bInRangeOfItem{false};
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
	UPROPERTY(EditAnywhere, Category = "Stats")
		bool bUseMousePosistion{ 0 };
	void RotateToMouse();
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
	//Level manager
	void GetLevelManager(class ALevelManager*);
	class ALevelManager* LevelManager;
	//Test save and loading
	void SaveGame();
	void LoadGame();
	//Decal stuff
	UPROPERTY(EditAnywhere)
	UDecalComponent* PlayerDecal;
	//Health packs
	void UseHealthPack();
	void GetHealthPack(float);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HealthPackCount{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthPackHealAmount{ 40 };
};
