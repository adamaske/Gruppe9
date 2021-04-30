// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyUnit.h"
#include "PlayerUnit.h"

void AKeyUnit::InteractWithPlayer(APlayerUnit* unit) {
	if (unit) {
		unit->GetKey();
		DestroyMe();
	}
	
}
