// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedEnemy.h"
<<<<<<< HEAD
#include "GameFramework/Controller.h"








void ABipedEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AIController = Cast<AAIController>(GetController());
=======
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"//GetController

>>>>>>> parent of fae415e (sad)

	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABipedEnemy::OnOverlap);
	SphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ABipedEnemy::OnOverlapEnd);
}
/*void  SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	SetupPlayerInputComponent(InputComponent);

}*/

ABipedEnemy::ABipedEnemy()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));
	
	BipedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Biped Mesh"));
	BipedMesh->SetupAttachment(RootComponent);

	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Trigger"));
	SphereTrigger->SetupAttachment(RootComponent);
	SphereTrigger->InitSphereRadius(600.f);
<<<<<<< HEAD

=======



}

void ABipedEnemy::BeginPlay()
{
	Super::BeginPlay();
>>>>>>> parent of fae415e (sad)


}

void ABipedEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APlayerUnit* Player = Cast<APlayerUnit>(OtherActor);
	if (Player)//checks if player char is present to avoid hard crashes
	{
		
		MoveToTarget(Player);
	}




}

void ABipedEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
<<<<<<< HEAD
	APlayerUnit* Player = Cast<APlayerUnit>(OtherActor);
	if (AIController && Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("out of unit area"));
		AIController->StopMovement();
	}
=======
>>>>>>> parent of fae415e (sad)
}

void  ABipedEnemy::MoveToTarget(APlayerUnit* PlayerUnit1)
{
	if (AIController)
	{


		FAIMoveRequest AIMoverequest;
		AIMoverequest.SetGoalActor(PlayerUnit);
		AIMoverequest.SetAcceptanceRadius(25.f);
<<<<<<< HEAD

		//creates a nav path
		FNavPathSharedPtr NavPtr;

		AIController->MoveTo(AIMoverequest, &NavPtr);


=======
>>>>>>> parent of fae415e (sad)
	}
}