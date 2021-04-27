// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableUnit.h"
#include "JournalTerminal.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AJournalTerminal : public AInteractableUnit
{
	GENERATED_BODY()
	
public:
	virtual void InteractWithPlayer(APlayerUnit*) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString TerminalName {"Journal Name"};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString TerminalText {"Journal Text"};

	void OpenTerminal(class APlayerUnit*);
	UPROPERTY(VisibleAnywhere)
	bool bIAmBeingRead{ 0 };
	void CloseTerminal(class APlayerUnit*);
};
