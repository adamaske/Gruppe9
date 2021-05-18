// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePointStation.h"
#include "PlayerUnit.h"

ASavePointStation::ASavePointStation() {
	ActiveIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("ActiveIndicator");
	ActiveIndicatorMesh->SetupAttachment(RootComponent);
	
}

void ASavePointStation::BeginPlay() {
	if (!bIAmCurrentSpawnPoint) {
		ActiveIndicatorMesh->SetVisibility(false);
		UE_LOG(LogTemp, Log, TEXT("Disabled spawn poit visiblity"));
	}
}

void ASavePointStation::InteractWithPlayer(APlayerUnit* player) {
	//Gives the player this as a savepointstation
	player->GetSpawnPointStation(this);
	bIAmCurrentSpawnPoint = true;
	if (bSaveGameOnActivatedSavePoint) {
		player->SaveGame();
	}
	ActiveIndicatorMesh->SetVisibility(true);
	UE_LOG(LogTemp, Log, TEXT("Save point set to this"));
}

void ASavePointStation::RemoveAsSavePointStation()
{
	bIAmCurrentSpawnPoint = false;
	ActiveIndicatorMesh->SetVisibility(false);
}
void ASavePointStation::SetToThisFromLoading(APlayerUnit* player)
{
	//Gives the player this as a savepointstation
	player->GetSpawnPointStation(this);
	bIAmCurrentSpawnPoint = true;
	ActiveIndicatorMesh->SetVisibility(true);
	UE_LOG(LogTemp, Log, TEXT("Save point set to this"));
}
void ASavePointStation::UsedFromLoader(bool DoSaving, APlayerUnit* player) {
	//Gives the player this as a savepointstation
	player->GetSpawnPointStation(this);
	bIAmCurrentSpawnPoint = true;
	/*if (bSaveGameOnActivatedSavePoint) {
		player->SaveGame();
	}*/
	ActiveIndicatorMesh->SetVisibility(true);
	UE_LOG(LogTemp, Log, TEXT("Save point set to this"));
}