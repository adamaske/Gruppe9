// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "SavePointStation.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API ASavePointStation : public AInteractableUnit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ActiveIndicatorMesh;
	ASavePointStation();
public:

	virtual void BeginPlay() override;
	virtual void InteractWithPlayer(class APlayerUnit*) override;

	void RemoveAsSavePointStation();

	UPROPERTY(VisibleAnywhere)
		bool bIAmCurrentSpawnPoint{ 0 };
	UPROPERTY(EditAnywhere)
		bool bSaveGameOnActivatedSavePoint{ 1 };
};
