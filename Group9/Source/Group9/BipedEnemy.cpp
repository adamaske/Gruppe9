// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedEnemy.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"







void ABipedEnemy::BeginPlay()
{
	//Super::BeginPlay();

	//AIController = Cast<AAIController>(GetController());
//GetController
}

ABipedEnemy::ABipedEnemy()
{
	//RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));
	
	//BipedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Biped Mesh"));
	//BipedMesh->SetupAttachment(RootComponent);




}

void ABipedEnemy::MoveUnit(FVector LookAtTarget)
{
}

void ABipedEnemy::Tick(float DeltaTime)
{
}


float ABipedEnemy::PlayerDistance()
{
	return 0.0f;
}
