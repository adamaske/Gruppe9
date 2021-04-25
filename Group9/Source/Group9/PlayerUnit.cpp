// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnit.h"
#include "InteractableUnit.h"
#include "SavePointStation.h"
#include "EnemyUnit.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
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

	
	MeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackCollision"));
	MeleeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerUnit::AttackHit);
	MeleeCollisionBox->SetupAttachment(RootComponent);
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
	PlayerInputComponent->BindAction("TestKey", IE_Pressed, this, &APlayerUnit::TakeDamageTest);
	PlayerInputComponent->BindAction("SaveTest", IE_Pressed, this, &APlayerUnit::SaveGame);
	PlayerInputComponent->BindAction("LoadTest", IE_Pressed, this, &APlayerUnit::LoadGame);
	PlayerInputComponent->BindAction("UseHealthPack", IE_Pressed, this, &APlayerUnit::UseHealthPack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerUnit::StartReload);
	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &APlayerUnit::StartMeleeAttack);
}

void APlayerUnit::MoveForward(float value)
{
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
	if (bIsReloading || bInMeleeAttack) {
		return;
	}
	if (CurrentMagazineAmmo > 0) {
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
					CurrentMagazineAmmo -= 1;
				}
			}
		}
	}
	else {
		StartReload();
	}
}

void APlayerUnit::StartReload() {
	if (!bIsReloading && CurrentAmmunition > 0 && !bInMeleeAttack && CurrentMagazineAmmo < MaxMagazineSize) {
		bIsReloading = true;
	}
}

void APlayerUnit::Reload(float DeltaTime) {
	//Check if it is already reloading
	if (CurrentAmmunition > 0) {
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

				currentReloadTime = 0;
				bIsReloading = false;
			}
		}
		else {
			currentReloadTime += DeltaTime;
		}
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
		UE_LOG(LogTemp, Log, TEXT("No LevelManager"));
		return;
	}
	LevelManager->Save();
}
void APlayerUnit::LoadGame() {
	if (!LevelManager) {
		UE_LOG(LogTemp, Log, TEXT("No LevelManager"));
		return;
	}
	LevelManager->Load();
}

void APlayerUnit::UseHealthPack()
{
	if (HealthPackCount > 0 && CurrentHealth < MaxHealth) {
		HealthPackCount -= 1;
		CurrentHealth += HealthPackHealAmount;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	}
}

void APlayerUnit::GetHealthPack(float amount)
{
	HealthPackCount += amount;
}

void APlayerUnit::StartMeleeAttack()
{
	if (!bInMeleeAttack && !bIsReloading) {
		bInMeleeAttack = true;
	}
}

void APlayerUnit::MeleeAttack(float DeltaTime)
{
	if (CurrentMeleeTime >= MeleeAttackTime) {
		//Do attack
		if (MeleeCollisionBox->GetGenerateOverlapEvents() == true) {
			MeleeCollisionBox->SetGenerateOverlapEvents(false);
			UE_LOG(LogTemp, Log, TEXT("Deactived melee box"));
		}
		bMeleeAttackHasHit = false;
		bInMeleeAttack = false;
		CurrentMeleeTime = 0;
	}
	else {
		//Only allow it to hit once in an atatck, if it has already hit, disable it
		if (!bMeleeAttackHasHit) {
			//If the timer is before or after the start and end, dont check for overlap events
			if (CurrentMeleeTime < MeleeAttackCollisionStart || CurrentMeleeTime >= MeleeAttackCollisionEnd) {
				//Deactivate collision
				if (MeleeCollisionBox->GetGenerateOverlapEvents() == true) {
					MeleeCollisionBox->SetGenerateOverlapEvents(false);
					UE_LOG(LogTemp, Log, TEXT("Deactived melee box"));
				}
			}
			//If the timer is between the start and end, check for collisions
			if (CurrentMeleeTime > MeleeAttackCollisionStart && CurrentMeleeTime < MeleeAttackCollisionEnd) {
				//Activate collison
				if (MeleeCollisionBox->GetGenerateOverlapEvents() == false) {
					MeleeCollisionBox->SetGenerateOverlapEvents(true);
					UE_LOG(LogTemp, Log, TEXT("Activated melee box"));
				}
			}
		}
		else {
			if (MeleeCollisionBox->GetGenerateOverlapEvents() == true) {
				MeleeCollisionBox->SetGenerateOverlapEvents(false);
				UE_LOG(LogTemp, Log, TEXT("Deactived melee box"));
			}
		}
		CurrentMeleeTime += DeltaTime;
	}
}

void APlayerUnit::AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bMeleeAttackHasHit) { 
		return; 
	}
	if (OtherActor->IsA(AEnemyUnit::StaticClass())) {
		bMeleeAttackHasHit = true;
		UE_LOG(LogTemp, Log, TEXT("Melee Attack Hit Enemy"));
		AEnemyUnit* unit = Cast<AEnemyUnit>(OtherActor);
		unit->TakeDamage(MeleeAttackDamage);
	}
}
