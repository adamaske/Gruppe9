// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableUnit.h"
#include "SavePointStation.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LevelManager.h"
#include "Bullet.h"
#include "Room.h"
// Sets default values
APlayerUnit::APlayerUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bInheritYaw = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 40.f,

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	MyCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	PlayerDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MouseDecal"));
	GetCharacterMovement()->MaxCustomMovementSpeed = MovementSpeed;
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

	if (bUseMousePosistion) {
		RotateToMouse();
	}
}

// Called to bind functionality to input
void APlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerUnit::Interact);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerUnit::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerUnit::MoveRight);
	if (!bUseMousePosistion) {
		PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	}
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerUnit::Shoot);
	PlayerInputComponent->BindAction("TestKey", IE_Pressed, this, &APlayerUnit::TakeDamageTest);
	PlayerInputComponent->BindAction("SaveTest", IE_Pressed, this, &APlayerUnit::SaveGame);
	PlayerInputComponent->BindAction("LoadTest", IE_Pressed, this, &APlayerUnit::LoadGame);
	
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

void APlayerUnit::RotateToMouse()
{
	if (PC) {
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if (!HitResult.GetActor()) {
			return;
		}
		FVector TempLocation = HitResult.Location;
		TempLocation.Z = GetActorLocation().Z;
		
		
		FVector NewDirection = TempLocation - GetActorLocation();

		if (FVector::Distance(GetActorLocation(), TempLocation) > 10) {
			NewDirection.Normalize();

			SetActorRotation(NewDirection.Rotation());
		}

		PlayerDecal->SetWorldLocation(HitResult.Location);
		PlayerDecal->SetWorldRotation(HitResult.Normal.Rotation());
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
	bInRangeOfItem = true;
}

void APlayerUnit::RemoveInteractableUnit(AInteractableUnit* unit)
{
	//If this is the current, set the current to null
	if (CurrentInteractableUnit == unit) {
		CurrentInteractableUnit = nullptr;
		bInRangeOfItem = false;
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

void APlayerUnit::TakeDamageTest()
{
	TakeDamage(10);
}

void APlayerUnit::Shoot() {

	if (CurrentAmmunition > 0) {
		if (ShootingTimer >= FireRate) {
			//Shoot
			UE_LOG(LogTemp, Log, TEXT("PLayer shot weapon"));
			ShootingTimer = 0;
			//If the bulletblueprint is alive
			if (BulletBlueprint) {
				//If it gets world
				UWorld* World = GetWorld();
				if (World) {
					//Spawn the bulletblueprint at actor location + 50cm forward, and with actor rotaiton
					World->SpawnActor<ABullet>(BulletBlueprint, GetActorLocation() + FVector(50.f, 0.f, 0.f), GetActorRotation());
					//use 1 ammo
					CurrentAmmunition -= 1;
				}
			}
		}
	}
	
}

void APlayerUnit::GetAmmunition(float value)
{
	CurrentAmmunition += value;
}

void APlayerUnit::GetRoom(ARoom* unit)
{
	//Gets a new Room and tells to update all its avalible rooms, so the enemy spawner gets correct info
	CurrentRoom = unit;
}
void APlayerUnit::GetLevelManager(ALevelManager* manager) {
	LevelManager = manager;
}

void APlayerUnit::SaveGame() {
	if (!LevelManager) {
		return;
	}
	LevelManager->Save();
}
void APlayerUnit::LoadGame() {
	if (!LevelManager) {
		return;
	}
	LevelManager->Load();
}