// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableUnit.h"
#include "SavePointStation.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
	PlayerMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerUnit::Interact);
}

void APlayerUnit::Interact() {
	//If the currentInteractleUnit is not a nullptr, call interact on it
	if (CurrentInteractableUnit) {
		CurrentInteractableUnit->InteractWithPlayer(this);
	}
}

void APlayerUnit::GetInteractableUnit(AInteractableUnit* unit)
{
	CurrentInteractableUnit = unit;
}

void APlayerUnit::RemoveInteractableUnit(AInteractableUnit* unit)
{
	//If this is the current, set the current to null
	if (CurrentInteractableUnit == unit) {
		CurrentInteractableUnit = nullptr;
	}
}

void APlayerUnit::GetSpawnPointStation(ASavePointStation* station)
{
	if (CurrentSavePointStation == station) {
		return;
	}
	if (CurrentSavePointStation != station) {
		if (CurrentSavePointStation) {
			CurrentSavePointStation->RemoveAsSavePointStation();
		}
		CurrentSavePointStation = station;
	}
}



