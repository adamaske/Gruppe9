// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableUnit.h"
#include "SavePointStation.h"
#include "EnemyUnit.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LevelManager.h"
#include "Bullet.h"
#include "Room.h"
#include "Camera/CameraShake.h"
#include "JournalTerminal.h"
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

	
	MeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackCollision"));
	MeleeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerUnit::AttackHit);
	MeleeCollisionBox->SetupAttachment(RootComponent);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
	GunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GunSocket"));
	FlashlightMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("FlashlightSocket"));
}

// Called when the game starts or when spawned
void APlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	PC = Cast<APlayerController>(GetController());
	
	MeleeCollisionBox->SetGenerateOverlapEvents(false);

}

// Called every frame
void APlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShootingTimer += DeltaTime;
	bTookDamage = false;
	if ( MovementVector.IsNearlyZero()) {
		bIsMoving = false;
	}
	else {
		bIsMoving = true;
	}
	if (bUseMousePosistion) {
		RotateToMouse();
	}
	if (bIsReloading) {
		Reload(DeltaTime);
	}
	if (bInMeleeAttack) {
		MeleeAttack(DeltaTime);
	}
	AmmoStringToDisplay = FString::SanitizeFloat(CurrentMagazineAmmo) + " / " + FString::SanitizeFloat(CurrentAmmunition);
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
	PlayerInputComponent->BindAction("SaveTest", IE_Pressed, this, &APlayerUnit::SaveGame);
	PlayerInputComponent->BindAction("LoadTest", IE_Pressed, this, &APlayerUnit::LoadGame);
	PlayerInputComponent->BindAction("UseHealthPack", IE_Pressed, this, &APlayerUnit::UseHealthPack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerUnit::StartReload);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &APlayerUnit::StartMeleeAttack);
}

void APlayerUnit::MoveForward(float value)
{
	if (ReadingJournalTerminal) {
		return;
	}
	MovementVector.X = value;
	FVector NewDirection;
	if (bUseLocalDirections) {
		NewDirection = GetActorForwardVector(); 
	}
	else {
		NewDirection = FVector::ForwardVector;
	}

	AddMovementInput(NewDirection, value);
}

void APlayerUnit::MoveRight(float value)
{
	if (ReadingJournalTerminal) {
		return;
	}
	MovementVector.Y = value;
	FVector NewDirection;
	if (bUseLocalDirections) {
		NewDirection = GetActorRightVector();
	}
	else {
		NewDirection = FVector::RightVector;
	}
	
	AddMovementInput(NewDirection, value);
}

void APlayerUnit::RotateToMouse()
{
	if (ReadingJournalTerminal) {
		return;
	}
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
	//Dont do anything if this is the current station
	if (CurrentSavePointStation == station) {
		return;
	}
	//If there is a current station, tell it its been removed as the current
	if (CurrentSavePointStation) {
		CurrentSavePointStation->RemoveAsSavePointStation();
	}
	//Set the new point
	CurrentSavePointStation = station;
	
}

void APlayerUnit::TakeDamage(float dmg)
{
	bTookDamage = true;
	CurrentHealth -= dmg;
	if (CurrentHealth <= 0) {
		//Set animation info, levelmanager takes care of the dying n stuff
		bIsDead = true;
	}
}

void APlayerUnit::Shoot() {
	//Dont allow to shoot if the player is in meele, reloading or reading a journal
	if (bIsReloading || bInMeleeAttack || ReadingJournalTerminal) {
		return;
	}
	//Check if enough ammo
	if (CurrentMagazineAmmo > 0) {
		//If the shooting timer is larger than firerate, it is allowed to shot
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
					ShakeCamera(false);
					World->SpawnActor<ABullet>(BulletBlueprint, GetActorLocation() + FVector(50.f, 0.f, 0.f), GetActorRotation());
					//use 1 ammo
					CurrentMagazineAmmo -= 1;
					PlaySound(ShootSound);
				}
			}
		}
	}
	else {
		//If there is not enough ammo in the magazine, start reloading
		StartReload();
	}
}

void APlayerUnit::StartReload() {
	//Check if you have enough ammo so that you can realod, and in melee and stuff, then set isrealodign to true
	if (!bIsReloading && CurrentAmmunition > 0 && !bInMeleeAttack && CurrentMagazineAmmo < MaxMagazineSize) {
		bIsReloading = true;
	}
}

void APlayerUnit::Reload(float DeltaTime) {
	//If the reloadtime has passed reloadtime, then do the actual reload, else + on deltatime
	if (currentReloadTime >= ReloadTime) {
		//Check if there is any more amunition
		if (CurrentAmmunition > 0) {
			//Find how much is missing from the current magazine
			float missingAmmo = MaxMagazineSize - CurrentMagazineAmmo;
			//Find if there is enough ammo to fill it, else take whats left
			if (missingAmmo > CurrentAmmunition) {
				missingAmmo = CurrentAmmunition;
			}
			//Add the ammo
			CurrentMagazineAmmo += missingAmmo;
			//FMath::Clamp(CurrentMagazineAmmo, 0.f, MaxMagazineSize);
			//Remove from ammo pool
			CurrentAmmunition -= missingAmmo;
			//Reset the timer
			currentReloadTime = 0;
			//Dont reload anymore
			bIsReloading = false;
			//Jucieness
			ShakeCamera(false);
			PlaySound(ReloadSound);
		}
	}
	else {
		currentReloadTime += DeltaTime;
	}
			
}

void APlayerUnit::GetAmmunition(float value)
{
	CurrentAmmunition += MaxMagazineSize;
}

void APlayerUnit::GetLevelManager(ALevelManager* manager) {
	LevelManager = manager;
}

void APlayerUnit::SaveGame() {
	if (!LevelManager) {
		return;
	}
	LevelManager->SaveTheGame();
}

void APlayerUnit::LoadGame() {
	if (!LevelManager) {
		return;
	}
	LevelManager->LoadTheGame();
}

void APlayerUnit::UseHealthPack()
{
	//Check if the player has any healthpacks
	if (HealthPackCount > 0 && CurrentHealth < MaxHealth) {
		//remove a healthpack
		HealthPackCount -= 1;
		//Add the healthpackamount to the health
		CurrentHealth += HealthPackHealAmount;
		//Clamp the health to not surpass max health
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	}
}

void APlayerUnit::GetHealthPack(float amount)
{
	HealthPackCount += amount;
}

void APlayerUnit::StartMeleeAttack()
{
	//Check if yourt allowed to melee attack
	if (!bInMeleeAttack && !bIsReloading) {
		bInMeleeAttack = true;
	}
}

void APlayerUnit::MeleeAttack(float DeltaTime)
{
	//Find how much time has gone
	if (CurrentMeleeTime >= MeleeAttackTime) {
		//Reset melee attack stuff if the time is over
		//If the box is generating overlap events, disable it
		if (MeleeCollisionBox->GetGenerateOverlapEvents() == true) {
			MeleeCollisionBox->SetGenerateOverlapEvents(false);
		}
		//Reset variables
		bMeleeAttackHasHit = false;
		bInMeleeAttack = false;
		CurrentMeleeTime = 0;
	}
	else {
		//ignore if already hit
		if (!bMeleeAttackHasHit) {
			//If the timer is before or after the start and end, dont check for overlap events
			if (CurrentMeleeTime < MeleeAttackCollisionStart || CurrentMeleeTime >= MeleeAttackCollisionEnd) {
				//Deactivate collision
				if (MeleeCollisionBox->GetGenerateOverlapEvents() == true) {
					MeleeCollisionBox->SetGenerateOverlapEvents(false);
				}
			}
			//If the timer is between the start and end, check for collisions
			if (CurrentMeleeTime > MeleeAttackCollisionStart && CurrentMeleeTime < MeleeAttackCollisionEnd) {
				//Activate collison
				if (MeleeCollisionBox->GetGenerateOverlapEvents() == false) {
					MeleeCollisionBox->SetGenerateOverlapEvents(true);			
				}
			}
		}
		else {
			//If it has already hit, deactivate the box
			if (MeleeCollisionBox->GetGenerateOverlapEvents() == true) {
				MeleeCollisionBox->SetGenerateOverlapEvents(false);
			}
		}
		//Add delta time to the timer
		CurrentMeleeTime += DeltaTime;
	}
}

void APlayerUnit::AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Only hit one enemy per attaack, 
	if (bMeleeAttackHasHit) { 
		return; 
	}
	//Find if the actor hit is an enemy
	if (OtherActor->IsA(AEnemyUnit::StaticClass())) {
		//Say you have already hit an enemy
		bMeleeAttackHasHit = true;
		//UE_LOG(LogTemp, Log, TEXT("Melee Attack Hit Enemy"));
		//Cast the actor to a enemyunit
		AEnemyUnit* unit = Cast<AEnemyUnit>(OtherActor);
		//Call damage on enemy 
		unit->TakeDamage(MeleeAttackDamage);
		//Juice
		PlaySound(MeleeSound);
		ShakeCamera(MeleeShake);
	}
}

void APlayerUnit::OpenTerminal(AJournalTerminal* terminal)
{
	//Set it to the current terminal
	CurrentTerminal = terminal;
	//Update variables
	JournalText = CurrentTerminal->TerminalText;
	JournalName = CurrentTerminal->TerminalName;
	//Is reading true
	ReadingJournalTerminal = true;
}

void APlayerUnit::CloseTerminal()
{
	//Disable journal variabls
	ReadingJournalTerminal = false;
	CurrentTerminal = nullptr;
}

void APlayerUnit::GetKey() {
	KeyAmount += 1;
}

bool APlayerUnit::UseKey() {
	//Remove key and return true if youre allowed open 
	if (KeyAmount > 0) {
		KeyAmount -= 1;
		return true;
	}
	else {
		return false;
	}
	
}

void APlayerUnit::ShakeCamera(TSubclassOf<class UCameraShakeBase> shaker)
{
	//Call shake on camera with the paramter
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(shaker, 1.f, ECameraShakePlaySpace::World);
}

void APlayerUnit::PlaySound(USoundBase* soundToPlay) {
	UGameplayStatics::PlaySound2D(GetWorld(), soundToPlay, 1.f, 1.f, 0.f);
};
