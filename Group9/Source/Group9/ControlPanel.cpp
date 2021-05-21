// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlPanel.h"
#include "TurretUnit.h"
#include "PlayerUnit.h"

AControlPanel::AControlPanel()
{
	//RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootCompt"));

	ControlPanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Control panel mesh"));
	ControlPanelMesh->SetupAttachment(RootComponent);
}

void AControlPanel::BeginPlay()
{
	Super::BeginPlay();
}

void AControlPanel::InteractWithPlayer(APlayerUnit* player)
{
	//Called when the player presses the interact key
	UE_LOG(LogTemp, Log, TEXT("Player interacted with me"));
	if (MyTurret) 
	{
		MyTurret->IAmActive = false;
	}
	
}
