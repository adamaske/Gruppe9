// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "SavePointStation.h"
#include "SaveManager.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "EnemySpawnPointUnit.h"
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

	if (bFindDoorsAndRoomsOnBeginPlay) {
		//Find doors and rooms
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);
		Doors.Empty();
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			Doors.Add(Cast<ADoor>(FoundActors[i]));
		}

		FoundActors.Empty();
		Rooms.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoom::StaticClass(), FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			Rooms.Add(Cast<ARoom>(FoundActors[i]));
		}
	}
	
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	if (MyController) {
		MyController->bShowMouseCursor = false;
		MyController->bEnableClickEvents = false;
		MyController->bEnableMouseOverEvents = false;
	}
	if (PlayerUnit) {
		PlayerUnit->GetLevelManager(this);
	}
	//Loads a save file if its found
	if (bLoadOnBeginPlay) {
		LoadTheGame();
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

	if (bPlayerIsDead) {
		if (CurrentDeathTimer >= DeathTimer) {
			CurrentDeathTimer = 0;
			bPlayerIsDead = false;
			PlayerDead();
		}
		else {
			CurrentDeathTimer += DeltaTime;
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

		bPlayerIsDead = true;
		
	}
	else if (PlayerUnit->bIsDead) {
		bPlayerIsDead = true;
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

void ALevelManager::PlayerDead()
{
	if (bLoadOnPlayerDeath) {
		LoadTheGame();
	}
}

void ALevelManager::SaveTheGame() {
	//Player stats
	USaveManager* PlayerStats = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	if (PlayerUnit) {
		PlayerStats->PlayerCurrentHealth = PlayerUnit->CurrentHealth;
		PlayerStats->PlayerHealthpackCount = PlayerUnit->HealthPackCount;

		PlayerStats->PlayerAmmoCount = PlayerUnit->CurrentAmmunition;
		PlayerStats->CurrentMagazineAmount = PlayerUnit->CurrentMagazineAmmo;

		PlayerStats->PlayerKeyAmount = PlayerUnit->KeyAmount;
	}

	//Level specific saving
	//Find and override the previous save file
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	
	//Find savepoint index
	for (int i{ 0 }; i < SaveStations.Num(); i++) {
		if (SaveStations[i]->bIAmCurrentSpawnPoint) {
			SaveGameInstance->CurrentSavePointIndex = i;
		}
	}
	//Find open doors
	for (int i{ 0 }; i < Doors.Num(); i++) {
		if (Doors[i]->bIsOpen) {
			SaveGameInstance->OpenDoorsIndexes.Add(i);
		}
	}
	//Find player location
	SaveGameInstance->PlayerLocation = PlayerUnit->GetActorLocation();

	if (!SaveGameInstance) {
		SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	}

	//Find what level you are on
	FString LevelNameToSave = "SaveFile" + UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	
	UGameplayStatics::SaveGameToSlot(PlayerStats, TEXT("PlayerStats"), 0);
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, LevelNameToSave, 0);
	UE_LOG(LogTemp, Log, TEXT("Saved Game"));
}
void ALevelManager::LoadTheGame() {
	//Player stats loading
	USaveManager* PlayerStats = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerStats"), 0));
	if (!PlayerStats) {
		return;
	};
	//Give the player stats
	if (PlayerUnit) {
		PlayerUnit->CurrentAmmunition = PlayerStats->PlayerAmmoCount;
		PlayerUnit->CurrentMagazineAmmo = PlayerStats->CurrentMagazineAmount;
		PlayerUnit->CurrentHealth = PlayerStats->PlayerCurrentHealth;
		PlayerUnit->HealthPackCount = PlayerStats->PlayerHealthpackCount;
		PlayerUnit->KeyAmount = PlayerStats->PlayerKeyAmount;
	}

	//Level specific loading
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	//Find name to load
	FString CurrentLevelName = "SaveFile" + UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	//Load from the name
	SaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(CurrentLevelName, 0));
	//Return if there is no savegame instance
	if (!SaveGameInstance) {
		UE_LOG(LogTemp, Log, TEXT("Found no Save Game File to Load"));
		return;
	}
	//Set save point station
	SaveStations[SaveGameInstance->CurrentSavePointIndex]->InteractWithPlayer(PlayerUnit);

	//Set open doors
	for (int i = 0; i < Doors.Num(); i++) {
		if (SaveGameInstance->OpenDoorsIndexes.Contains(i)) {
			UE_LOG(LogTemp, Log, TEXT("Opened door at index %i"), i);
			Doors[i]->OpenDoor();
		}
	}

	//Set player location
	PlayerUnit->SetActorLocation(SaveGameInstance->PlayerLocation);
	
	UE_LOG(LogTemp, Log, TEXT("Loaded Game"));
}