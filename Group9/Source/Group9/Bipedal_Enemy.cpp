// Fill out your copyright notice in the Description page of Project Settings.


#include "Bipedal_Enemy.h"
#include "AIController.h"
#include "EnemyUnit.h"
#include "PlayerUnit.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABipedal_Enemy::ABipedal_Enemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootCompBiped"));

	BipedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BipedkjasdMesh"));
	BipedMesh->SetupAttachment(RootComponent);

	MeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeCollider"));
	MeleeBox->OnComponentBeginOverlap.AddDynamic(this, &ABipedal_Enemy::MeleeHit);
	MeleeBox->SetupAttachment(RootComponent);

	DashMeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DashCollider"));
	DashMeleeBox->OnComponentBeginOverlap.AddDynamic(this, &ABipedal_Enemy::DashHit);
	DashMeleeBox->SetupAttachment(RootComponent);

}

void ABipedal_Enemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MeleeBox->SetGenerateOverlapEvents(false);
	DashMeleeBox->SetGenerateOverlapEvents(false);

}

void  ABipedal_Enemy::MoveUnit(FVector LookAtTarget)
{
	/*/rotates to face player
	FVector FaceTarget = FVector(LookAtTarget.X, LookAtTarget.Y, GetActorLocation().Z);
	FVector StartLocation = GetActorLocation();

	FRotator RotateBiped = FVector(FaceTarget - StartLocation).Rotation();
	SetActorRotation(RotateBiped);*/

	//move to player
	LookAtTarget.Z = GetActorLocation().Z;
	FVector NewDirection = LookAtTarget - GetActorLocation();
	NewDirection.Normalize();

	SetActorRotation(NewDirection.Rotation());

	FVector MoveVector = GetActorLocation() + GetActorForwardVector() * movementSpeed * 0.1;
	SetActorLocation(MoveVector);

}



void ABipedal_Enemy::CloseMeleeAttack(float DeltaTime)
{
	//finds how much has gone to determine the attack rate
	if (CurrentTime >= MeleeHitTime)
	{
		//resets melee attack timer after its done 1 cycle
		//disables melee box collider if overlap
		if (MeleeBox->GetGenerateOverlapEvents() == true)
		{
			MeleeBox->SetGenerateOverlapEvents(false);
		}
		//resets melee attack
		bMeleeHit = false;
		bIsAttacking = false;
		CurrentTime = 0;
	}
	else
	{//ignore if enemy has allready struck player
		if (!bMeleeHit)
		{//if the timer is outside the set attack cycle, then dont check overlap
			if (CurrentTime < MeleeStart || CurrentTime >= MeleeEnd)
			{	//deactive collison box
				if (MeleeBox->GetGenerateOverlapEvents() == true)
				{
					MeleeBox->SetGenerateOverlapEvents(false);
				}
			}
			//if the timer is inside the melee cycle then check for overlap
			if (CurrentTime > MeleeStart && CurrentTime < MeleeEnd)
			{
				if (MeleeBox->GetGenerateOverlapEvents() == false)
				{

					MeleeBox->SetGenerateOverlapEvents(true);
				}
			}
		}
		else
		{//if melee attack has occured in current cycle, disable box to prevent damage stacking
			if (MeleeBox->GetGenerateOverlapEvents() == true)
			{
				MeleeBox->SetGenerateOverlapEvents(false);
			}
		}
		//adds deltatime to timer for melee cycle
		CurrentTime += DeltaTime;
	}
}

void ABipedal_Enemy::MeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//prevents hit stacking
	if (bMeleeHit)
	{
		return;
	}
	//checks if hit overlaps with playerUnit
	if (OtherActor->IsA(APlayerUnit::StaticClass()))
	{
		bMeleeHit = true;
		UE_LOG(LogTemp, Log, TEXT("Dash Attack Hit Player"))
			//casts to plyer unit
			APlayerUnit* playerUnit = Cast<APlayerUnit>(OtherActor);
		playerUnit->TakeDamage(MeleeDamage);
	}
}

void ABipedal_Enemy::DashMeleeAttack(float DeltaTime)
{
	CurrentDashChargeTime += DeltaTime;
	if (CurrentDashChargeTime >= DashChargeTime + DashMovementEnd)
	{
		//UE_LOG(LogTemp, Log, TEXT(" Not DASHING"));
		//Stop moving
		//Stop attacking
		if (DashMeleeBox->GetGenerateOverlapEvents() == true)
		{
			DashMeleeBox->SetGenerateOverlapEvents(false);
		}
		//resets melee attack
		UE_LOG(LogTemp, Log, TEXT(" sets to false"));
		bDashhit = false;
		bIsCharging = false;
		CurrentDashChargeTime = 0;
		DashStart = 0;
		DashHitTime = 0;
		bIsDashing = false;

		return;



	}
	if (CurrentDashChargeTime >= DashChargeTime)
	{
		//UE_LOG(LogTemp, Log, TEXT("DASHING"));
		//FVector playerLocation = PlayerUnit->GetActorLocation();
		bIsDashing = true;
		FVector ChargeVector = GetActorLocation() + (GetActorForwardVector() * DashSpeed * DeltaTime);
		SetActorLocation(ChargeVector);
		if (DashMeleeBox->GetGenerateOverlapEvents() == false)
		{
			UE_LOG(LogTemp, Log, TEXT("sets overlap to true"));
			DashMeleeBox->SetGenerateOverlapEvents(true);
			//
		}

	}

	/*else
	{//if melee attack has occured in current cycle, disable box to prevent damage stacking
		if (DashMeleeBox->GetGenerateOverlapEvents() == true)
		{
			DashMeleeBox->SetGenerateOverlapEvents(false);
		}
	}*/
}

void ABipedal_Enemy::DashHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Dash hit event firing"));
	//prevents hit stacking
	if (bDashhit)
	{
		UE_LOG(LogTemp, Log, TEXT("bDashHit return event"))
			return;
	}
	//checks if hit overlaps with playerUnit
	if (OtherActor->IsA(APlayerUnit::StaticClass()))
	{
		bDashhit = true;
		UE_LOG(LogTemp, Log, TEXT("Dash Attack Hit Player"))
			//casts to plyer unit
			APlayerUnit* playerUnit = Cast<APlayerUnit>(OtherActor);
		playerUnit->TakeDamage(DashDamage);
	}
}

void ABipedal_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Dont do anything if tghere is no player refrence
	if (!PlayerUnit)
	{
		return;
	}

	//Gets location of player and biped to measure the distance between them
	FVector currentPlayerLocal = PlayerUnit->GetActorLocation();
	FVector currentBipedLocal = GetActorLocation();

	float MeleeRange = FVector::Distance(currentPlayerLocal, currentBipedLocal);
	if (bIsCharging)
	{
		DashMeleeAttack(DeltaTime);
		return;
	}
	if (bIsAttacking)
	{
		CloseMeleeAttack(DeltaTime);
		return;
	}
	// checks if playerunit exist to avoid hard crash, and player distance
	if (BipedStopRange < MeleeRange && bIsAttacking == false && bIsCharging == false)
	{
		MoveUnit(PlayerUnit->GetActorLocation());

	}
	//UE_LOG(LogTemp, Log, TEXT("Current distance from player: %f"), MeleeRange);
	if (MeleeRange <= BipedStopRange)
	{
		//UE_LOG(LogTemp, Log, TEXT("Melee triggered"));
		bIsAttacking = true;
		//CloseMeleeAttack(DeltaTime);
		return;
	}

	if (DashRange >= MeleeRange)
	{
		//UE_LOG(LogTemp, Log, TEXT("Melee triggered"));
		bIsCharging = true;
		DashMeleeAttack(DeltaTime);
		return;
	}
}
float ABipedal_Enemy::PlayerDistance()
{
	return 0.0f;
}


