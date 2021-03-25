// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
class UProjectileMovementComponent;
UCLASS()
class GROUP9_API AEnemyBullet : public ABullet
{
	GENERATED_BODY()

		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Bullet stats")
	UProjectileMovementComponent* BulletMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Bullet stats")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Bullet stats")
	TSubclassOf<UDamageType> BulletDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Bullet stats")
	float BulletSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Bullet stats")
	float TurretDamage = 25;

public:
	AEnemyBullet();



};
