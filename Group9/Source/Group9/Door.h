// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API ADoor : public AInteractableUnit
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* DoorMesh;
public:
	ADoor();
	void OpenDoor();

	bool bIsOpen;

	UPROPERTY(EditAnywhere)
	class ARoom* RoomBehindMe;

	void InteractWithPlayer(class APlayerUnit*);

	
};
