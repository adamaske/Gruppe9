// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "Room.h"
#include "Door.h"
// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Find all rooms
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);

	for (AActor* ac : FoundActors) {
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

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	currentTime += DeltaTime;
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

void ALevelManager::DoSpawning() {
	UE_LOG(LogTemp, Log, TEXT("Called spawn function"));
	//Check for dead enemies, destroy and remove them
	for (int i = 0; i < AliveEnemies.Num(); i++)
	{
		if (AliveEnemies[i]->bIsDead) {
			AliveEnemies[i]->Destroy();
			AliveEnemies.RemoveAt(i);
			CurrentEnemiesCount--;
		}
	}

	//Get all open rooms, dont want enemies to spawn behind closed doors
	TArray<ARoom*> OpenRooms;
	for (int i{ 0 }; i < Rooms.Num(); i++)
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
	
	UWorld* world = GetWorld();
	if (world && EnemyBlueprint) {
		//For each of the to spawn foudn earlier
		for (int i = 0; i < ToSpawn; i++)
		{
			//Get a random spawn point 
			int newIndex = FMath::RandRange(0, AvailableSpawnPoints.Num()-1);
			//Store it in a actor
			AActor* currentSpawnPoint = AvailableSpawnPoints[newIndex];
			AEnemyUnit* newEnemy = world->SpawnActor<AEnemyUnit>(EnemyBlueprint, currentSpawnPoint->GetActorLocation(), FRotator(0.f, 0.f, 0.f));
			//Add it to alive enemies
			AliveEnemies.Add(newEnemy);
			//Increase enemy count
			CurrentEnemiesCount++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Spawned enemies: %d"), ToSpawn);
}

