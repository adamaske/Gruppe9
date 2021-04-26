// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "EnemyTurret.h"
#include "FuseBox.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AFuseBox : public AInteractableUnit
{
	GENERATED_BODY()

		AFuseBox();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuse Box")
		UStaticMeshComponent* FuseBoxMesh;

		virtual void InteractWithPlayer(APlayerUnit* player) override;

		AEnemyTurret* myTurret;
};
