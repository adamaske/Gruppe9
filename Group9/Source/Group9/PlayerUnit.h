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

	void Interact();

	UPROPERTY(VisibleAnywhere, Category = "Interacting")
		class AInteractableUnit* CurrentInteractableUnit{ nullptr };

	//Interacting with Interactables
	void GetInteractableUnit(AInteractableUnit* unit);
	void RemoveInteractableUnit(AInteractableUnit* unit);
	//Spawn points
	void GetSpawnPointStation(class ASavePointStation* station);
	class ASavePointStation* CurrentSavePointStation{ nullptr };
private:
	class AInteractableUnit* InteractableUnitInRange{nullptr};
};
