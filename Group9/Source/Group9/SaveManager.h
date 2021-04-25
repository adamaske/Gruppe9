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
	//Location
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector PlayerLocation;
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
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int CurrentSavePointIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int> OpenDoorsIndexes;
};
