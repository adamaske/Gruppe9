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
	if (PlayerUnit) {
		PlayerUnit->GetLevelManager(this);
	}
	//Loads a save file if its found
	if (bLoadOnBeginPlay) {
		Load();
	}
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
		if (bLoadOnPlayerDeath) {
			Load();
			return;
		}
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
	USaveManager* StatsSaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	if (PlayerUnit) {
		StatsSaveGameInstance->PlayerLocation = PlayerUnit->GetActorLocation();

		StatsSaveGameInstance->PlayerCurrentHealth = PlayerUnit->CurrentHealth;
		StatsSaveGameInstance->PlayerHealthpackCount = PlayerUnit->HealthPackCount;

		StatsSaveGameInstance->PlayerAmmoCount = PlayerUnit->CurrentAmmunition;
		StatsSaveGameInstance->CurrentMagazineAmount = PlayerUnit->CurrentMagazineAmmo;

		//Find open doors
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (Doors[i]->bIsOpen) {
				SaveGameInstance->OpenDoorsIndexes.Add(i);
			}
		}

		//Find current Savepoint
		for (int i = 0; i < SaveStations.Num(); i++)
		{
			if (SaveStations[i]->bIAmCurrentSpawnPoint) {
				SaveGameInstance->CurrentSavePointIndex = i;
			}
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveFileName, 0);
	UGameplayStatics::SaveGameToSlot(StatsSaveGameInstance, "StatsSave", 0);
	UE_LOG(LogTemp, Log, TEXT("Saved Game"));
}
void ALevelManager::Load() {
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	SaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(SaveFileName, 0));
	USaveManager* StatsSaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	StatsSaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot("StatsSave", 0));
	if (!SaveGameInstance || !StatsSaveGameInstance) {
		UE_LOG(LogTemp, Log, TEXT("Found no Save Game File to Load"));
		return;
	}
	if (PlayerUnit) {
		PlayerUnit->SetActorLocation(StatsSaveGameInstance->PlayerLocation);

		PlayerUnit->CurrentAmmunition = StatsSaveGameInstance->PlayerAmmoCount;
		PlayerUnit->CurrentMagazineAmmo = StatsSaveGameInstance->CurrentMagazineAmount;

		PlayerUnit->CurrentHealth = StatsSaveGameInstance->PlayerCurrentHealth;
		PlayerUnit->HealthPackCount = StatsSaveGameInstance->PlayerHealthpackCount;

		//Open doors
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (SaveGameInstance->OpenDoorsIndexes.Contains(i)) {
				Doors[i]->OpenDoor();
			}
		}

		//Set active spawnpoint
		if (SaveGameInstance->CurrentSavePointIndex) {
			SaveStations[SaveGameInstance->CurrentSavePointIndex]->InteractWithPlayer(PlayerUnit);
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded Game"));
	}
	
}