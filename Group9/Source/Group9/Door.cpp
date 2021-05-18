// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PlayerUnit.h"
#include "Room.h"
ADoor::ADoor() {
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	OpenDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenDoorMesh"));
	OpenDoorMesh->SetupAttachment(RootComponent);
	OpenDoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OpenDoorMesh->SetVisibility(false);
}
void ADoor::OpenDoor() {

	bIsOpen = true;
	if (bSwapMeshes) {
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DoorMesh->SetVisibility(false);

		OpenDoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OpenDoorMesh->SetVisibility(true);
	}
	else {
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DoorMesh->SetVisibility(false);
	}
	if (RoomBehindMe) {
		RoomBehindMe->bRoomIsOpen = true;
	}
	bIgnoreCollision = true;
	RemoveMeAsInteractableNow();
	UE_LOG(LogTemp, Log, TEXT("Opened door"));
	

}

void ADoor::InteractWithPlayer(APlayerUnit* unit) {
	if (unit->UseKey()) {
		OpenDoor();
	}
}


