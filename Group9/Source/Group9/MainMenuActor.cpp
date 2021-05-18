// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuActor.h"
#include "Kismet/GameplayStatics.h"
#include "SaveManager.h"
// Sets default values
AMainMenuActor::AMainMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	if (MyController) {
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}
	

	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
	SaveGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerStats"), 0));

	if (!SaveGameInstance || SaveGameInstance->CurrentLevelName == "") {
		LevelToLoad = "/Game/Maps/Kalle";
	}
	else {
		LevelToLoad = "/Game/Maps/" + SaveGameInstance->CurrentLevelName;
	}
}

// Called every frame
void AMainMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

