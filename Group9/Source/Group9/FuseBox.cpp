// Fill out your copyright notice in the Description page of Project Settings.


#include "FuseBox.h"

AFuseBox::AFuseBox()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComp"));

	FuseBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fuse Box Mesh"));
	FuseBoxMesh->SetupAttachment(RootComponent); 
}



void AInteractableUnit::InteractWithPlayer(APlayerUnit* player)
{
	AEnemyTurret* myTurret;

	//Called when the player presses the interact key
	UE_LOG(LogTemp, Log, TEXT("Player interacted with me"));
	
	myTurret->myBox = false;
}
