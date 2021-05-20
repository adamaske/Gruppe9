// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "EnemyUnit.h"
#include "BipedUnit.h"
#include "PlayerUnit.h"
#include "InteractableUnit.h"
void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->IsA(AEnemyUnit::StaticClass())) {
		AEnemyUnit* unit = Cast<AEnemyUnit>(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Player bullet hit enemy"));
		unit->TakeDamage(damage);
		this->Destroy();
	}
	else if (OtherActor->IsA(ABipedUnit::StaticClass())) {
		ABipedUnit* unit = Cast<ABipedUnit>(OtherActor);
		unit->TakeDamage(damage);
		this->Destroy();
	}
	else if(OtherActor->IsA(APlayerUnit::StaticClass())){
		UE_LOG(LogTemp, Log, TEXT("Bullet hit owner"));
	}
	else if (OtherActor->IsA(AInteractableUnit::StaticClass())) {

	}
	else {
		this->Destroy();
	}
	
}