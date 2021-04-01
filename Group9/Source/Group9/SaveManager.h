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
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerAmmoCount;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerHealthpackCount;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerCurrentHealth;
};
