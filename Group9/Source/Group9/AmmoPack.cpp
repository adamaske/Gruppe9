// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPack.h"
#include "PlayerUnit.h"
void AAmmoPack::InteractWithPlayer(APlayerUnit* unit)
{
	//Calls get ammo to the player
	unit->GetAmmunition(AmmoAmount);
	UE_LOG(LogTemp, Log, TEXT("Picked up ammo"));
	//destroys itself, since it wont be used anymore
	DestroyMe();
}
