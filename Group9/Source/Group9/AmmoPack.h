// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "AmmoPack.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AAmmoPack : public AInteractableUnit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float AmmoAmount{10};
	virtual void InteractWithPlayer(class APlayerUnit*) override;
};
