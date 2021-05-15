// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class GROUP9_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		class APlayerUnit* myPlayer;

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties();
};
