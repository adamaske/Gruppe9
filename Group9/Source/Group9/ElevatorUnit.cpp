// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorUnit.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"
void AElevatorUnit::InteractWithPlayer(APlayerUnit* player) {
	UWorld* world = GetWorld();

	UGameplayStatics::OpenLevel(world, FName(MapToLoadName));
}