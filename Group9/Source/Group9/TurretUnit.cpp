// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretUnit.h"
#include "PlayerUnit.h"
#include "EnemyBullet.h"
#include "Kismet/GameplayStatics.h"
ATurretUnit::ATurretUnit()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));

	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("turret base mesh"));
	TurretBaseMesh->SetupAttachment(RootComponent);
	
	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("turret head mesh"));
	TurretHeadMesh->SetupAttachment(TurretBaseMesh);
	EnemyBulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Bullet spawn point"));
	EnemyBulletSpawnPoint->SetupAttachment(TurretHeadMesh);
}

void ATurretUnit::BeginPlay()
{
	Super::BeginPlay();
	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


void ATurretUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// checks if playerunit exist to avoid hard crash, and player distance
	if (!IAmActive) {
		return;
	}
	if (!PlayerUnit || PlayerDistance() > TurretRange)
	{
		return;
	}

	// rotates the mesh of the turret to face player
	TurretRotate(PlayerUnit->GetActorLocation());
	CanShoot(DeltaTime);
}

float ATurretUnit::PlayerDistance()
{
	return FVector::Distance(PlayerUnit->GetActorLocation(), GetActorLocation());
}


void ATurretUnit::TurretFire()
{
	//if statement to protecct form crashes in case somne commponents are missing
		if (EnemyBulletClass)
		{
			//gets the vecot location of the bullet spawn point game object
			FVector BulletSpawnLocation = EnemyBulletSpawnPoint->GetComponentLocation();
			//gets the vector rotation of the bullet spawn point game object
			FRotator BulletSpawnRotation = EnemyBulletSpawnPoint->GetComponentRotation();
			//pointer to bullet spawns the bullet at the retrieved vecotrs
			AEnemyBullet* TempBullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulletClass, BulletSpawnLocation, BulletSpawnRotation);
	
			//Makes it so that the bullet doesnt damage the turret
			TempBullet->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("fire!!!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("No EnemyBulletClass"));
		}
		
}

void ATurretUnit::TurretRotate(FVector LookAtTarget)
{
	//checks if the turret is active by control panel.cpp
	if (IAmActive)
	{
		FVector AimAtTarget = FVector(LookAtTarget.X, LookAtTarget.Y, TurretHeadMesh->GetComponentLocation().Z);
		FVector StartLocation = TurretHeadMesh->GetComponentLocation();
		//rotates 
		FRotator RotateTurret = FVector(AimAtTarget - StartLocation).Rotation();
		RotateTurret.Yaw += 180;
		TurretHeadMesh->SetWorldRotation(RotateTurret);
	}
}

void ATurretUnit::CanShoot(float DeltaTime)
{
	//Chekcs if the player exist
	if (!PlayerUnit)
	{
		return;
	}
	currentShootTime += DeltaTime;
	//if player is in range, open fire and IAmActive and current shoottime is higher than fire rate.
	if (PlayerDistance() <= TurretRange && IAmActive && currentShootTime > FireRate)
	{
		//fire
		//UE_LOG(LogTemp, Warning, TEXT("Turret in range!"));
		TurretFire();
		currentShootTime = 0;
	}
}