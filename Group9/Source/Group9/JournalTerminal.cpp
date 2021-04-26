// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalTerminal.h"
#include "PlayerUnit.h"

void AJournalTerminal::InteractWithPlayer(APlayerUnit* player) {
	if (bIAmBeingRead) {
		CloseTerminal(player);
	}
	else {
		OpenTerminal(player);
	}
}

void AJournalTerminal::OpenTerminal(APlayerUnit* player)
{
	bIAmBeingRead = true;
	player->OpenTerminal(this);
}

void AJournalTerminal::CloseTerminal(APlayerUnit* player) {
	bIAmBeingRead = false;
	player->CloseTerminal();
}