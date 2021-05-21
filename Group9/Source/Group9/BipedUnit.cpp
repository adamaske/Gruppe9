// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedUnit.h"
#include "AIController.h"
#include "EnemyUnit.h"
#include "PlayerUnit.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ABipedUnit::ABipedUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// sets movement speed of character
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	//Constructor for the normal melee attack collider 
	MeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeCollider"));
	MeleeBox->OnComponentBeginOverlap.AddDynamic(this, &ABipedUnit::MeleeHit);
	MeleeBox->SetupAttachment(GetRootComponent());
	//Collider for dash attack 
	DashMeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DashCollider"));
	DashMeleeBox->OnComponentBeginOverlap.AddDynamic(this, &ABipedUnit::DashHit);
	DashMeleeBox->SetupAttachment(DashMeleeBox);
}

void ABipedUnit::BeginPlay()
{
	Super::BeginPlay();
	//retrieves the AIController
	AIController = Cast<AAIController>(GetController());
	//retrives player unit 
	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));
	MeleeBox->SetGenerateOverlapEvents(false);
	DashMeleeBox->SetGenerateOverlapEvents(false);
}
void ABipedUnit::CloseMeleeAttack(float DeltaTime)
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
		AnimIsAttacking = false;
		CurrentTime = 0;
		
		return;
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
void ABipedUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABipedUnit::MeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
//take damage func from enemy unit
void ABipedUnit::TakeDamage(float dmg)
{
	UE_LOG(LogTemp, Log, TEXT("Enemy took damage"));
	CurrentHealth -= dmg;
	if (CurrentHealth <= 0) 
	{
		//Die
		AnimDeath = true;
		UE_LOG(LogTemp, Log, TEXT("Enemy dead"));
		bIsDead = true;

		if (bIsDead && bDestroyMyselfOnDeath) {
			this->Destroy();
		}
	}
}

void ABipedUnit::DashMeleeAttack(float DeltaTime)
{
	//adds time for dash charge time
	CurrentDashChargeTime += DeltaTime;
	AnimIsCharging = true;
	if (CurrentDashChargeTime >= DashChargeTime + DashMovementEnd)
	{
		//UE_LOG(LogTemp, Log, TEXT(" Not DASHING"));
		//Stop moving
		//Stop attacking
		if (DashMeleeBox->GetGenerateOverlapEvents() == true)
		{
			DashMeleeBox->SetGenerateOverlapEvents(false);
		}
		//resets melee attack and BP checks
		UE_LOG(LogTemp, Log, TEXT(" sets to false"));
		bDashhit = false;
		bIsCharging = false;
		CurrentDashChargeTime = 0;
		DashStart = 0;
		DashHitTime = 0;
		bIsDashing = false;
		AnimIsDashing = false;
		AnimDasAttacking = false;
		AnimIsCharging = false;
		HasDashed = true;
		return;



	}
	if (CurrentDashChargeTime >= DashChargeTime)
	{
		//dashing to true to apply damage
		bIsDashing = true;
		//BP anim bool
		AnimIsDashing = true;
		//sets target for dashing
		FVector ChargeVector = GetActorLocation() + (GetActorForwardVector() * DashSpeed * DeltaTime);
		//enitiates dashing
		SetActorLocation(ChargeVector);
		//Bp animation bool
		AnimDasAttacking = true;
		//if the collision on the dash collier is false set to true to damage player
		if (DashMeleeBox->GetGenerateOverlapEvents() == false)
		{
			UE_LOG(LogTemp, Log, TEXT("sets overlap to true"));
			DashMeleeBox->SetGenerateOverlapEvents(true);
			
		}
	}
}
//overlap event for dash to damage player
void ABipedUnit::DashHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ABipedUnit::Tick(float DeltaTime)
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
	
	//checks if the nit is close enough to normal melee attack
	if (MeleeRange <= BipedStopRange)
	{
		//UE_LOG(LogTemp, Log, TEXT("Melee triggered"));
		bIsAttacking = true;
		AnimIsAttacking = true;
		CloseMeleeAttack(DeltaTime);
		return;
	}
	//checks if the unit can dash by distance check and if it has dashed
	if (MeleeRange <= DashRange && !HasDashed)
	{
		//UE_LOG(LogTemp, Log, TEXT("Melee triggered"));
		bIsCharging = true;
		DashMeleeAttack(DeltaTime);
		return;
	}
	// checks if playerunit exist to avoid hard crash, and player distance
	if (bIsAttacking == false && bIsCharging == false)
	{
		AnimIsWalking = true;
		AIController->MoveToActor(PlayerUnit, 1.f);
	}
}
float ABipedUnit::PlayerDistance()
{
	return 0.0f;
}