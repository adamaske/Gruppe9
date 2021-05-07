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
			LoadTheGame();
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

void ALevelManager::SaveTheGame() {
	//Find and override the previous save file
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveFile"), 0));
	if (!SaveGameInstance) {
		SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	}

	UE_LOG(LogTemp, Log, TEXT("Started saving"));
	if (PlayerUnit) {
		//Only find the statics of the player, and save what level they are on
		SaveGameInstance->PlayerLocation = PlayerUnit->GetActorLocation();

		SaveGameInstance->PlayerCurrentHealth = PlayerUnit->CurrentHealth;
		SaveGameInstance->PlayerHealthpackCount = PlayerUnit->HealthPackCount;

		SaveGameInstance->PlayerAmmoCount = PlayerUnit->CurrentAmmunition;
		SaveGameInstance->CurrentMagazineAmount = PlayerUnit->CurrentMagazineAmmo;

		SaveGameInstance->CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		SaveGameInstance->PlayerKeyAmount = PlayerUnit->KeyAmount;
	}

	
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
			if (SaveGameInstance->CurrentLevelName == "Level1") {
				UE_LOG(LogTemp, Log, TEXT("Saved a savepoitnstation 1 index"));
				SaveGameInstance->Level1SavePointIndex = i;
				SaveGameInstance->LevelHasIndex = true;
			}
			else if (SaveGameInstance->CurrentLevelName == "Level2") {
				UE_LOG(LogTemp, Log, TEXT("Saved a savepoitnstation 2 index"));
				SaveGameInstance->Level2SavePointIndex = i;
				SaveGameInstance->Level2HasIndex = true;
			}
		}
	}


	//Save location
	if (SaveGameInstance->CurrentLevelName == "Level1") {
		SaveGameInstance->Level1PlayerLocation = PlayerUnit->GetActorLocation();
		//Find open doors
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (Doors[i]->bIsOpen) {
				SaveGameInstance->Level1DoorsIndexes.Add(i);
			}
		}
	}
	else if (SaveGameInstance->CurrentLevelName == "Level2") {
		SaveGameInstance->Level2PlayerLocation = PlayerUnit->GetActorLocation();
		//Find open doors
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (Doors[i]->bIsOpen) {
				SaveGameInstance->Level2DoorsIndexes.Add(i);
			}
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveFile"), 0);
	UE_LOG(LogTemp, Log, TEXT("Saved Game"));
}
void ALevelManager::LoadTheGame() {

	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	SaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveFile"), 0));
	//Return if there is no savegame instance
	if (!SaveGameInstance) {
		UE_LOG(LogTemp, Log, TEXT("Found no Save Game File to Load"));
		return;
	}
	//Give the player stats
	if (PlayerUnit) {
		PlayerUnit->CurrentAmmunition = SaveGameInstance->PlayerAmmoCount;
		PlayerUnit->CurrentMagazineAmmo = SaveGameInstance->CurrentMagazineAmount;
		PlayerUnit->CurrentHealth = SaveGameInstance->PlayerCurrentHealth;
		PlayerUnit->HealthPackCount = SaveGameInstance->PlayerHealthpackCount;
		PlayerUnit->KeyAmount = SaveGameInstance->PlayerKeyAmount;
	}
	
	//Dont do anything from here incase there is no player
	if (!PlayerUnit) {
		return;
	}
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (CurrentLevelName == "Level1") {
		if (!SaveGameInstance->LevelHasIndex) {
			return;
		}

		//Open doors
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (SaveGameInstance->Level1DoorsIndexes.Contains(i)) {
				Doors[i]->OpenDoor();
			}
		}
		SaveStations[SaveGameInstance->Level1SavePointIndex]->UsedFromLoader(true, PlayerUnit);
		PlayerUnit->SetActorLocation(SaveGameInstance->Level1PlayerLocation);

	}
	else if (CurrentLevelName == "Level2") {
		if (!SaveGameInstance->Level2HasIndex) {
			return;
		}

		//Open doors
		for (int i = 0; i < Doors.Num(); i++)
		{
			if (SaveGameInstance->Level2DoorsIndexes.Contains(i)) {
				Doors[i]->OpenDoor();
			}
		}
		SaveStations[SaveGameInstance->Level2SavePointIndex]->UsedFromLoader(true, PlayerUnit);
		PlayerUnit->SetActorLocation(SaveGameInstance->Level2PlayerLocation);

	}
	

	UE_LOG(LogTemp, Log, TEXT("Loaded Game"));
}