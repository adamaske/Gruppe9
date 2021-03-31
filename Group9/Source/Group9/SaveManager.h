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
	FVector PlayerLocation;
	float PlayerAmmoCount;
	float PlayerHealthpackCount;
	float PlayerCurrentHealth;
};
