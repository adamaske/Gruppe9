// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "HealthPackUnit.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AHealthPackUnit : public AInteractableUnit
{
	GENERATED_BODY()
public:
	virtual void InteractWithPlayer(class APlayerUnit*) override;
};
