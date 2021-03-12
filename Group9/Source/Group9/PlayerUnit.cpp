// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableUnit.h"
#include "SavePointStation.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bullet.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
	PlayerMesh->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bInheritYaw = false;
	CameraBoom->bEnableCameraLag = true;

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	MyCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	PC = Cast<APlayerController>(GetController());
	
}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShootingTimer += DeltaTime;
}

// Called to bind functionality to input
void APlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerUnit::Interact);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerUnit::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerUnit::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerUnit::Shoot);
}

void APlayerUnit::MoveForward(float value)
{
	FVector NewDirection;
	if (bCameraForward) {

		FRotator tempRotation = CameraBoom->GetComponentRotation();
		tempRotation -= GetViewRotation();
		tempRotation.Pitch = 0.f;		// we don't want the pitch, only yaw
		NewDirection = tempRotation.Vector();
	}
	else {
		NewDirection = GetActorForwardVector();
	}
	MovementVector.X = value;
	AddMovementInput(NewDirection, value);
}

void APlayerUnit::MoveRight(float value)
{
	FVector NewDirection;
	if (bCameraForward) {

		FRotator tempRotation = CameraBoom->GetComponentRotation();
		tempRotation -= GetViewRotation();
		tempRotation.Pitch = 0.f;		// we don't want the pitch, only yaw
		NewDirection = tempRotation.Vector();
	}
	else {
		NewDirection = GetActorRightVector();
	}
	MovementVector.X = value;
	AddMovementInput(NewDirection, value);
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

void APlayerUnit::TakeDamage(float dmg)
{
	CurrentHealth -= dmg;
	if (CurrentHealth <= 0) {
		//Die
	}
}

void APlayerUnit::Shoot() {
	if (ShootingTimer >= FireRate) {
		//Shoot
		UE_LOG(LogTemp, Error, TEXT("PLayer shot weapon"));
		ShootingTimer = 0;

		if (BulletBlueprint) {
			UWorld* World = GetWorld();
			if (World) {
				World->SpawnActor<ABullet>(BulletBlueprint, GetActorLocation(), GetActorRotation());
			}
		}
	}
}