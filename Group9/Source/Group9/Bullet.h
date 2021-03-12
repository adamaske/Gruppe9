// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class GROUP9_API ABullet : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* meshComponent;
public:	
	// Sets default values for this actor's properties
	ABullet();

	UPROPERTY(EditAnywhere, Category = "Stats")
		float ProjectileSpeed{ 700 };

	UPROPERTY(EditAnywhere, Category = "Stats")
		float allowedLifetime{ 5 };

	UPROPERTY(EditAnywhere)
		float damage{ 50 };

	float timeLived{ 0 };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float);

	void Lifetime(float);

	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
};
