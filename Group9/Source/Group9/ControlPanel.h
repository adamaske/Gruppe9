// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "ControlPanel.generated.h"


UCLASS()
class GROUP9_API AControlPanel : public AInteractableUnit
{
	GENERATED_BODY()


public:
	AControlPanel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Panel")
		UStaticMeshComponent* ControlPanelMesh;

	

protected:
	virtual void BeginPlay() override;

	virtual void InteractWithPlayer(class ATurretUnit*);
};
