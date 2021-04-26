// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePointStation.h"
#include "PlayerUnit.h"

void ASavePointStation::InteractWithPlayer(APlayerUnit* player) {
	//Gives the player this as a savepointstation
	player->GetSpawnPointStation(this);
	bIAmCurrentSpawnPoint = true;
	UE_LOG(LogTemp, Log, TEXT("Save point set to this"));
}

void ASavePointStation::RemoveAsSavePointStation()
{
	bIAmCurrentSpawnPoint = false;
}
