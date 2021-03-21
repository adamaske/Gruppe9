// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Group9GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GROUP9_API AGroup9GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		AGroup9GameModeBase();
public:


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	

};
