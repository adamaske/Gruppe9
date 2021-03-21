// Copyright Epic Games, Inc. All Rights Reserved.


#include "Group9GameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "Room.h"
#include "Door.h"
AGroup9GameModeBase::AGroup9GameModeBase() {
	PrimaryActorTick.bCanEverTick = true;

};

void AGroup9GameModeBase::BeginPlay() {
	Super::BeginPlay();
}

void AGroup9GameModeBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

