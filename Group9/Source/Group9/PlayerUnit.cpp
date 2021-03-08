// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableUnit.h"
#include "SavePointStation.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
	PlayerMesh->SetupAttachment(RootComponent);
	/*DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("MousePosistionDecal"));
	DecalComponent->SetupAttachment(RootComponent);*/
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	PC = Cast<APlayerController>(GetController());

	if (!bUseMousePositionAsForward) {
		//DecalComponent->Deactivate();
	}
	
}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	DoMovement(DeltaTime);
}

// Called to bind functionality to input
void APlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerUnit::Interact);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerUnit::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerUnit::MoveRight);
	PlayerInputComponent->BindAction("TakeSomeDamageTest", IE_Pressed, this, &APlayerUnit::TakeSomeDamageTest);
	PlayerInputComponent->BindAxis("MoveToCursor", this, &APlayerUnit::SetMoveLocation);
}

void APlayerUnit::DoMovement(float DeltaTime)
{
	if (bUseMousePositionAsForward) {
		
		FVector TempLocation = FVector(CurrentMoveLocation.X, CurrentMoveLocation.Y, GetActorLocation().Z);
		FVector NewDirection = TempLocation - GetActorLocation();

		if (FVector::Distance(GetActorLocation(), TempLocation) >= 1.f) {
			//DecalComponent->SetWorldLocation(HitResult.Location);
			NewDirection.Normalize();

			SetActorRotation(NewDirection.Rotation());

			FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * MovementSpeed * DeltaTime);

			SetActorLocation(NewLocation);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Distance between player and move is smaller than 1"));
		}
	}
	else {
		if (!MovementVector.IsZero()) {
			FVector newLocation = GetActorLocation() + (MovementVector * MovementSpeed * DeltaTime);

			SetActorLocation(newLocation);
		}
	}

	
}

void APlayerUnit::MoveForward(float value)
{
	MovementVector.X = value;
}

void APlayerUnit::MoveRight(float value)
{
	MovementVector.Y = value;
}

void APlayerUnit::SetMoveLocation(float value)
{
	if (value == 1) {
		if (PC) {
			FHitResult HitResult;
			if (PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult)) {
				CurrentMoveLocation = HitResult.Location;
			}
		}
	}
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


void APlayerUnit::TakeSomeDamageTest() {
	this->TakeDamage(10);
}
void APlayerUnit::TakeDamage(float dmg)
{
	CurrentHealth -= dmg;
	if (CurrentHealth <= 0) {
		//Die
	}
}

