// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"

// Called when the game starts or when spawned
void AEnemyTurret::BeginPlay()
{
	Super::BeginPlay();

	//starts countdown for turret firerate
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AEnemyTurret::CanShoot, FireRate, true);

	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));

	
}

AEnemyTurret::AEnemyTurret()
{	
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));

	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("turret base mesh"));
	TurretBaseMesh->SetupAttachment(RootComponent);

	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("turret head mesh"));
	TurretHeadMesh->SetupAttachment(TurretBaseMesh);
	EnemyBulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Bullet spawn point"));
	EnemyBulletSpawnPoint->SetupAttachment(TurretHeadMesh);
}

void AEnemyTurret::TurretFire()
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

void AEnemyTurret::TurretRotate(FVector LookAtTarget)
{
	FVector AimAtTarget = FVector(LookAtTarget.X, LookAtTarget.Y, TurretHeadMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretHeadMesh->GetComponentLocation();

	FRotator RotateTurret = FVector(AimAtTarget - StartLocation).Rotation();// +FRotator(0.f, 90.f, 0.f);
	TurretHeadMesh->SetWorldRotation(RotateTurret);

}

// Called every frame
void AEnemyTurret::Tick(float DeltaTime)
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
void AEnemyTurret::HandleDestruction()
{

}

/*void AEnemyTurret::Rotate()
{
	

}*/
void AEnemyTurret::CanShoot()
{
	//Chekcs if the player exist
	if (!PlayerUnit)
	{
		return;
	}

	//if player is in range, open fire
	if (PlayerDistance() <= TurretRange)
	{
		//fire
		//allways send out a raycast from bullet spawn point, if it DEOSNT hit the player it dont shoot innit.
		UE_LOG(LogTemp, Warning, TEXT("Turret in range!"));
		TurretFire();
	}
}

float AEnemyTurret::PlayerDistance()
{
	if (!PlayerUnit)
	{
		return 0.0f;
	}
	return FVector::Dist(PlayerUnit->GetActorLocation(), GetActorLocation());
}