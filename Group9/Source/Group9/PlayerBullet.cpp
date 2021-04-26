// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "EnemyUnit.h"
void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->IsA(AEnemyUnit::StaticClass())) {
		AEnemyUnit* unit = Cast<AEnemyUnit>(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Player bullet hit enemy"));
		unit->TakeDamage(damage);
		this->Destroy();
	}
	
}