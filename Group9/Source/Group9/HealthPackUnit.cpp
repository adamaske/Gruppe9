// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPackUnit.h"
#include "PlayerUnit.h"

void AHealthPackUnit::InteractWithPlayer(APlayerUnit* player) {
	player->GetHealthPack(1);
	DestroyMe();
}