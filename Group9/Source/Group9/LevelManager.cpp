// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "SavePointStation.h"
#include "SaveManager.h"
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
	PlayerUnit->GetLevelManager(this);
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckEnemies();
	CheckPlayer();
	if (bDoSpawning) {
		currentTime += DeltaTime;
		if (PlayerUnit) {
			if (currentTime >= Cooldown) {
				DoSpawning();
				currentTime = 0;
			}
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("LevelManager no player"));
		}
	}
}

void ALevelManager::CheckEnemies() {
	//Check for dead enemies, destroy and remove them
	for (int i = 0; i < AliveEnemies.Num(); i++)
	{
		if (AliveEnemies[i]->bIsDead) {
			AliveEnemies[i]->Destroy();
			AliveEnemies.RemoveAt(i);
			CurrentEnemiesCount--;
		}
	}

}

void ALevelManager::CheckPlayer()
{
	//Check player death
	if (PlayerUnit->GetActorLocation().Z <= KillPlayerZValue) {

		UE_LOG(LogTemp, Log, TEXT("Player under kill Z value"));
		if (PlayerUnit->CurrentSavePointStation) {
			ASavePointStation* PlayersSavePoint = PlayerUnit->CurrentSavePointStation;
			FVector NewLocation = PlayersSavePoint->GetActorLocation();
			NewLocation.X += 100;
			PlayerUnit->SetActorLocation(NewLocation);
		}
		else {
			FVector NewLocation = FVector(0.f, 0.f, 0.f);
			PlayerUnit->SetActorLocation(NewLocation);
		}
		
	}

}

void ALevelManager::DoSpawning() {
	UE_LOG(LogTemp, Log, TEXT("Called spawn function"));
	
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
			newEnemy->PlayerUnit = PlayerUnit;
			//Add it to alive enemies
			AliveEnemies.Add(newEnemy);
			//Increase enemy count
			CurrentEnemiesCount++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Spawned enemies: %d"), ToSpawn);
}

void ALevelManager::Save() {
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	if (PlayerUnit) {
		SaveGameInstance->PlayerLocation = PlayerUnit->GetActorLocation();
		SaveGameInstance->PlayerAmmoCount = PlayerUnit->CurrentAmmunition;
		SaveGameInstance->PlayerCurrentHealth = PlayerUnit->CurrentHealth;
		UE_LOG(LogTemp, Log, TEXT("Saved player posision %s"), *SaveGameInstance->PlayerLocation.ToString());
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
}
void ALevelManager::Load() {
	//USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));

	if (!SaveGameInstance) {
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded player ammo %f"), SaveGameInstance->PlayerAmmoCount);
	if (PlayerUnit) {
		UE_LOG(LogTemp, Log, TEXT("Loaded player posision %s"), *SaveGameInstance->PlayerLocation.ToString());
		//PlayerUnit->SetActorLocation(SaveGameInstance->PlayerLocation);
		PlayerUnit->CurrentAmmunition = SaveGameInstance->PlayerAmmoCount;
		PlayerUnit->CurrentHealth = SaveGameInstance->PlayerCurrentHealth;
	}
	
}