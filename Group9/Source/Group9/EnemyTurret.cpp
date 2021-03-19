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
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	TurretBaseMesh->SetupAttachment(RootComponent);
	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	TurretHeadMesh->SetupAttachment(RootComponent);
}

// Called every frame
void AEnemyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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