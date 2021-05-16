// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveManager.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API USaveManager : public USaveGame
{
	GENERATED_BODY()

public:
	//Player stuff
	//Health
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerCurrentHealth;
	//Ammunition
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerAmmoCount;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float CurrentMagazineAmount;
	//Health packs
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerHealthpackCount;
	//Keys
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int PlayerKeyAmount;
	//CurrentLevel
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString CurrentLevelName;


	//Level Specific stuff
	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<int> DoorsIndexes;
	//Location
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FVector PlayerLocation;
	//Savepoint
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int CurrentSavePointIndex;
	//Doors that are open
	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<int> OpenDoorsIndexes;

};
