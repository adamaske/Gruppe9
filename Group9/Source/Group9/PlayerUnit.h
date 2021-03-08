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
		UStaticMeshComponent* PlayerMesh;
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
	void DoMovement(float);
	void MoveForward(float);
	void MoveRight(float);
	void SetMoveLocation(float);
	FVector CurrentMoveLocation;
	FVector MovementVector{ 0.f,0.f,0.f };
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MovementSpeed{ 20 };
	UPROPERTY(EditAnywhere, Category = "Stats")
		bool bUseMousePositionAsForward{1};
	APlayerController* PC{nullptr};

	//Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float MaxHealth{ 100 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float CurrentHealth{ 100 };

	void TakeDamage(float damage);

	void TakeSomeDamageTest();

	//Decal on mouse posistion
	UPROPERTY(EditAnywhere)
	UDecalComponent* DecalComponent;
};
