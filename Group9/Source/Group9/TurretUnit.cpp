// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretUnit.h"
#include "PlayerUnit.h"
#include "EnemyBullet.h"
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
}


void ATurretUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// checks if playerunit exist to avoid hard crash, and player distance
		if (!PlayerUnit || PlayerDistance() > TurretRange)
		{
		return;
		}
		// rotates the mesh of the turret to face player
		TurretRotate(PlayerUnit->GetActorLocation());
}

float ATurretUnit::PlayerDistance()
{
	return 0.0f;
}


void ATurretUnit::TurretFire()
{
	//if statement to protecct form crashes in case somne commponents are missing
		if (EnemyBulletClass)
		{
			FVector BulletSpawnLocation = EnemyBulletSpawnPoint->GetComponentLocation();
			FRotator BulletSpawnRotation = EnemyBulletSpawnPoint->GetComponentRotation();
	
			AEnemyBullet* TempBullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulletClass, BulletSpawnLocation, BulletSpawnRotation);
	
			//Makes it so that the bullet doesnt damage the turret
			//TempBullet->SetOwner(this);
		}
		UE_LOG(LogTemp, Warning, TEXT("fire!!!"));
}

void ATurretUnit::TurretRotate(FVector LookAtTarget)
{
	if (IAmActive)
	{
		FVector AimAtTarget = FVector(LookAtTarget.X, LookAtTarget.Y, TurretHeadMesh->GetComponentLocation().Z);
		FVector StartLocation = TurretHeadMesh->GetComponentLocation();
		
		FRotator RotateTurret = FVector(AimAtTarget - StartLocation).Rotation();
		TurretHeadMesh->SetWorldRotation(RotateTurret);
	}
}

void ATurretUnit::HandleDestruction()
{
}
void ATurretUnit::CanShoot()
{
	//Chekcs if the player exist
	if (!PlayerUnit)
	{
		return;
	}

	//if player is in range, open fire
	if (PlayerDistance() <= TurretRange && IAmActive)
	{
		//fire
		//allways send out a raycast from bullet spawn point, if it DEOSNT hit the player it dont shoot innit.
		UE_LOG(LogTemp, Warning, TEXT("Turret in range!"));
		TurretFire();
	}
}