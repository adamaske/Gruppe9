// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedEnemy.h"
#include "Kismet/GameplayStatics.h"







void ABipedEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


ABipedEnemy::ABipedEnemy()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));
	
	BipedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Biped Mesh"));
	BipedMesh->SetupAttachment(RootComponent);

}

void  ABipedEnemy::MoveUnit(FVector LookAtTarget)
{

			FVector FaceTarget = FVector(LookAtTarget.X, LookAtTarget.Y, BipedMesh->GetComponentLocation().Z);
			FVector StartLocation = BipedMesh->GetComponentLocation();

			FRotator RotateBiped = FVector(FaceTarget - StartLocation).Rotation();
			BipedMesh->SetWorldRotation(RotateBiped);

			
			LookAtTarget.Z = GetActorLocation().Z;
			FVector NewDirection = LookAtTarget - GetActorLocation();
			NewDirection.Normalize();

			
			SetActorRotation(NewDirection.Rotation());


			FVector MoveVector = GetActorLocation() + GetActorForwardVector() * movementSpeed* 0.1;
			SetActorLocation(MoveVector);
		
}



void ABipedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Gets location of player and biped to measure the distance between them
	FVector currentPlayerLocal = PlayerUnit->GetActorLocation();
	FVector currentBipedLocal = BipedMesh->GetComponentLocation();
	float MeleeRange = FVector::Distance(currentPlayerLocal, currentBipedLocal);


	// checks if playerunit exist to avoid hard crash, and player distance
	if (!PlayerUnit || BipedStopRange < MeleeRange)
	{
		MoveUnit(PlayerUnit->GetActorLocation());
		
	}
	
	
	
}
