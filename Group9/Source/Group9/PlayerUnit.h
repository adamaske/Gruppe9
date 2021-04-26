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
	bool bUseLocalDirections = false;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MaxMagazineSize{ 20 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentMagazineAmmo{ 20 };
	void GetAmmunition(float);
	//Reloading
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bIsReloading{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float ReloadTime{ 2.f };
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		float currentReloadTime{ 0 };

	void Reload(float);
	void StartReload();
	UPROPERTY(BlueprintReadOnly)
	FString AmmoStringToDisplay{""};
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

	//Melee attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bInMeleeAttack;
	UPROPERTY(EditAnywhere, Category  = "Combat")
	class UBoxComponent* MeleeCollisionBox;
	void StartMeleeAttack();
	void MeleeAttack(float);
	UPROPERTY(EditAnywhere, Category = "Combat")
	float CurrentMeleeTime{0};
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeAttackTime{1};
	UPROPERTY(EditAnywhere, Category = "Combat")
		float MeleeAttackCollisionStart{ 0.65f };
	UPROPERTY(EditAnywhere, Category = "Combat")
		float MeleeAttackCollisionEnd{ 0.85f };
	bool bMeleeAttackHasHit{0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float MeleeAttackDamage{ 50 };
	UFUNCTION()
		void AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	//Terminal reading
	void OpenTerminal(class AJournalTerminal*);
	AJournalTerminal* CurrentTerminal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool ReadingJournalTerminal{ 0 };
	void CloseTerminal();

};
