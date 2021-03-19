// Copyright Epic Games, Inc. All Rights Reserved.


#include "Group9GameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"
#include "Room.h"
#include "Door.h"
AGroup9GameModeBase::AGroup9GameModeBase() {
	PrimaryActorTick.bCanEverTick = true;

};

void AGroup9GameModeBase::BeginPlay() {
	Super::BeginPlay();

	//Find all rooms
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);
	
	for (AActor* ac: FoundActors) {
		ADoor* newDoor = Cast<ADoor>(ac);
		if (newDoor) {
			Doors.Add(newDoor);
		}
	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoom::StaticClass(), FoundActors);
	for (AActor* ac : FoundActors) {
		ARoom* newRoom = Cast<ARoom>(ac);
		if (newRoom) {
			Rooms.Add(newRoom);
		}
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);
	PlayerUnit = Cast<APlayerUnit>(FoundActors[0]);
}

void AGroup9GameModeBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);


	currentTime += DeltaSeconds;
	if (PlayerUnit) {
		if (currentTime >= Cooldown) {
			DoSpawning();
			currentTime = 0;
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Gamemode no player"));
	}
}


void AGroup9GameModeBase::DoSpawning() {
	UE_LOG(LogTemp, Log, TEXT("Called spawn function"));
	
	//Get all open rooms, dont want enemies to spawn behind closed doors
	TArray<ARoom*> OpenRooms;
	for (int i{0}; i < Rooms.Num(); i++)
	{
		if (Rooms[i]->bRoomIsOpen) {
			OpenRooms.Add(Rooms[i]);
		}
	}
	//Get all the spawnpoints in the open rooms
	TArray<AActor*> AvailableSpawnPoints;
	for (int i{ 0 }; i < OpenRooms.Num(); i++)
	{
		for (int j{ 0 }; j < OpenRooms[i]->SpawnPoints.Num(); j++)
		{
			AvailableSpawnPoints.Add(OpenRooms[i]->SpawnPoints[j]);
		}
	}

	//Get the max new amount to spawn
	float MaxNewEnemies = MaxEnemiesCount - CurrentEnemiesCount;
	//Decide new amount
	float AmountNewToSpawn = FMath::RandRange(0.f, MaxNewEnemies);
	//turn to int
	int ToSpawn = FMath::RoundToInt(AmountNewToSpawn);
}