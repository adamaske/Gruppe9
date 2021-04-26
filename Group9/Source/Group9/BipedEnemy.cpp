// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedEnemy.h"
<<<<<<< HEAD
#include "Kismet/GameplayStatics.h"







void ABipedEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerUnit = Cast<APlayerUnit>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


=======
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"//GetController




>>>>>>> 9d25d68e37dfaefc7bcc0f8bdba1364341a6377e
ABipedEnemy::ABipedEnemy()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));
	
	BipedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Biped Mesh"));
	BipedMesh->SetupAttachment(RootComponent);

}

<<<<<<< HEAD
void  ABipedEnemy::MoveUnit(FVector LookAtTarget)
=======
void ABipedEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AIController = Cast<AAIController>(GetController());
}


void ABipedEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
>>>>>>> 9d25d68e37dfaefc7bcc0f8bdba1364341a6377e
{

			FVector FaceTarget = FVector(LookAtTarget.X, LookAtTarget.Y, BipedMesh->GetComponentLocation().Z);
			FVector StartLocation = BipedMesh->GetComponentLocation();

			FRotator RotateBiped = FVector(FaceTarget - StartLocation).Rotation();
			BipedMesh->SetWorldRotation(RotateBiped);

			
			LookAtTarget.Z = GetActorLocation().Z;
			FVector NewDirection = LookAtTarget - GetActorLocation();
			NewDirection.Normalize();

			
			SetActorRotation(NewDirection.Rotation());


<<<<<<< HEAD
			FVector MoveVector = GetActorLocation() + GetActorForwardVector() * movementSpeed* 0.1;
			SetActorLocation(MoveVector);
		
=======
void ABipedEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
>>>>>>> 9d25d68e37dfaefc7bcc0f8bdba1364341a6377e
}


<<<<<<< HEAD

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
		
=======
		FAIMoveRequest AIMoverequest;
		AIMoverequest.SetGoalActor(PlayerUnit);
		AIMoverequest.SetAcceptanceRadius(25.f);
>>>>>>> 9d25d68e37dfaefc7bcc0f8bdba1364341a6377e
	}
	
	
	
}
