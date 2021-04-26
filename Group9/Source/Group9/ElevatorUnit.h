// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "ElevatorUnit.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AElevatorUnit : public AInteractableUnit
{
	GENERATED_BODY()

	virtual void InteractWithPlayer(class APlayerUnit*) override;

	UPROPERTY(EditAnywhere)
	FString MapToLoadName{"Level2"};
};
