// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "EnemyBullet.h"
#include "EnemyTurret.generated.h"

class APlayerUnit;
class AEnemyBullet;

UCLASS()
class GROUP9_API AEnemyTurret : public AEnemyUnit
{
	GENERATED_BODY()
	
private:


	//void Rotate();
	FQuat Rotation;

	APlayerUnit* PlayerUnit;


	void CanShoot();
	

	FTimerHandle FireRateTimerHandle;

	float PlayerDistance();


public:
		
	AEnemyTurret();

	virtual void Tick(float DeltaTime) override;

		//Here we create two meshes for the turret, the idea is that when the turret rotates, only the head moves, while the base remain static
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Components")
		UStaticMeshComponent* TurretBaseMesh;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Components")
		UStaticMeshComponent* TurretHeadMesh;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Components")
			USceneComponent* EnemyBulletSpawnPoint;
		

		UPROPERTY(EditAnywhere, Category = "Turret Stats")
			float  TurretRotationSpeed{ 300 };

		UPROPERTY(EditAnywhere, Category = "Turret Stats")
			float TurretHealth{ 100 };

		UPROPERTY(EditAnywhere, Category = "Turret Stats")
			float FireRate{ 2.0f };

		UPROPERTY(EditAnywhere, Category = "Turret Stats")
			float TurretRange{ 500.f };
		
		
		//bullet actor
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret Stats")
		TSubclassOf<AEnemyBullet> EnemyBulletClass;

protected:

		virtual void BeginPlay() override;

		void TurretFire();

		void TurretRotate(FVector LookAtTarget);

		virtual void HandleDestruction() override;



};


