// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "GameFrameWork/ProjectileMovementComponent.h"



AEnemyBullet::AEnemyBullet()
{
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet Movement"));
	BulletMovement->InitialSpeed = BulletSpeed;
	BulletMovement->MaxSpeed = BulletSpeed;
	InitialLifeSpan = 5.0f;
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Enemy bullet hit something"));

}