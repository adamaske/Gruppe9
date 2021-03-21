// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PlayerUnit.h"
#include "Room.h"
ADoor::ADoor() {
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
}
void ADoor::OpenDoor() {

	bIsOpen = true;
	if (RoomBehindMe) {
		RoomBehindMe->bRoomIsOpen = true;
	}
	bIgnoreCollision = true;
	RemoveMeAsInteractableNow();
	UE_LOG(LogTemp, Log, TEXT("Opened door"));
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DoorMesh->SetVisibility(false);

}

void ADoor::InteractWithPlayer(APlayerUnit* unit) {
	OpenDoor();
}

