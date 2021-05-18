// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedEnemy.h"
#include "AIController.h"
#include "EnemyUnit.h"
#include "PlayerUnit.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



ABipedEnemy::ABipedEnemy()
{
	//bPrimaryActorCanTick = true;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));

	BipedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Biped Mesh"));
	BipedMesh->SetupAttachment(RootComponent);

	MeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeCollider"));
	MeleeBox->OnComponentBeginOverlap.AddDynamic(this, &ABipedEnemy::MeleeHit);
	MeleeBox->SetupAttachment(RootComponent);

	DashMeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DashCollider"));
	DashMeleeBox->OnComponentBeginOverlap.AddDynamic(this, &ABipedEnemy::MeleeHit);
	DashMeleeBox->SetupAttachment(RootComponent);
}

void ABipedEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MeleeBox->SetGenerateOverlapEvents(false);

}

void  ABipedEnemy::MoveUnit(FVector LookAtTarget)
{
	//rotates to face player
	FVector FaceTarget = FVector(LookAtTarget.X, LookAtTarget.Y, GetActorLocation().Z);
	FVector StartLocation = GetActorLocation();

	FRotator RotateBiped = FVector(FaceTarget - StartLocation).Rotation();
	SetActorRotation(RotateBiped);

	//move to player
	LookAtTarget.Z = GetActorLocation().Z;
	FVector NewDirection = LookAtTarget - GetActorLocation();
	NewDirection.Normalize();

	SetActorRotation(NewDirection.Rotation());

	FVector MoveVector = GetActorLocation() + GetActorForwardVector() * movementSpeed * 0.1;
	SetActorLocation(MoveVector);

}



void ABipedEnemy::CloseMeleeAttack(float DeltaTime)
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

void ABipedEnemy::MeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		UE_LOG(LogTemp, Log, TEXT("Melee Attack Hit Player"))
			//casts to plyer unit
			APlayerUnit* playerUnit = Cast<APlayerUnit>(OtherActor);
		playerUnit->TakeDamage(MeleeDamage);
	}
}

void ABipedEnemy::DashMeleeAttack(float DeltaTime)
{
	
	if (CurrentDashChargeTime >= DashChargeTime + DashMovementEnd)
	{
		//Stop moving
		//Stop attacking
		if (DashMeleeBox->GetGenerateOverlapEvents() == true)
		{
			DashMeleeBox->SetGenerateOverlapEvents(false);
		}
		//resets melee attack
		bDashhit = false;
		bIsCharging = false;
		CurrentDashChargeTime = 0;
		bIsCharging = false;
		return;
	}
	if (CurrentDashChargeTime >= DashChargeTime)
	{
		//CurrentDashChargeTime += DeltaTime;

		UE_LOG(LogTemp, Log, TEXT("DASHING"));
		//FVector playerLocation = PlayerUnit->GetActorLocation();
		FVector ChargeVector = GetActorForwardVector() * movementSpeed;
		AddActorLocalOffset(-ChargeVector);
	}
	else 
	{
		CurrentDashChargeTime += DeltaTime;
	}
	//finds how much has gone to determine the attack rate
	/*
	else
	{//ignore if enemy has allready struck player
		if (!bDashhit)
		{//if the timer is outside the set attack cycle, then dont check overlap
			if (CurrentDashChargeTime < DashStart || CurrentDashChargeTime >= DashEnd)
			{	//deactive collison box
				if (DashMeleeBox->GetGenerateOverlapEvents() == true)
				{
					DashMeleeBox->SetGenerateOverlapEvents(false);
				}
			}
			//if the timer is inside the melee cycle then check for overlap
			if (CurrentDashChargeTime > DashStart && CurrentDashChargeTime < DashEnd)
			{
				if (DashMeleeBox->GetGenerateOverlapEvents() == false)
				{

					DashMeleeBox->SetGenerateOverlapEvents(true);
				}
			}
		}
		else
		{//if melee attack has occured in current cycle, disable box to prevent damage stacking
			if (DashMeleeBox->GetGenerateOverlapEvents() == true)
			{
				DashMeleeBox->SetGenerateOverlapEvents(false);
			}
		}
		//adds deltatime to timer for melee cycle
		CurrentDashChargeTime += DeltaTime;
	}*/


}

void ABipedEnemy::DashHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABipedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Dont do anything if tghere is no player refrence
	if (!PlayerUnit) {
		return;
	}
	//Gets location of player and biped to measure the distance between them
	FVector currentPlayerLocal = PlayerUnit->GetActorLocation();
	FVector currentBipedLocal = GetActorLocation();
	float MeleeRange = FVector::Distance(currentPlayerLocal, currentBipedLocal);
	if (bIsCharging) {
		DashMeleeAttack(DeltaTime);
		return;
	}
	if (bIsAttacking) {
		CloseMeleeAttack(DeltaTime);
		return;
	}
	// checks if playerunit exist to avoid hard crash, and player distance
	if (BipedStopRange < MeleeRange && bIsAttacking == false && bIsCharging == false)
	{
		MoveUnit(PlayerUnit->GetActorLocation());

	}
	UE_LOG(LogTemp, Log, TEXT("Current distance from player: %f"), MeleeRange);
	if (MeleeRange <= BipedStopRange)
	{
		//UE_LOG(LogTemp, Log, TEXT("Melee triggered"));
		bIsAttacking = true;
		//CloseMeleeAttack(DeltaTime);
		return;
	}
	if (DashRange >= MeleeRange)
	{
		UE_LOG(LogTemp, Log, TEXT("Melee triggered"));
		bIsCharging = true;
		DashMeleeAttack(DeltaTime);
		return;
	}
}
float ABipedEnemy::PlayerDistance()
{
	return 0.0f;
}
