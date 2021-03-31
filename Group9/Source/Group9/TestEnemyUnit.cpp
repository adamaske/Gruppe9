// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemyUnit.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"

ATestEnemyUnit::ATestEnemyUnit() {

}
void ATestEnemyUnit::Tick(float DeltaSeconds) {
	Movement(DeltaSeconds);
}

void ATestEnemyUnit::Movement(float DeltaSeconds)
{
	//Move to player
	if (PlayerUnit) {
		FVector NewDirection = GetActorLocation() - PlayerUnit->GetActorLocation();
		NewDirection.Normalize();

		SetActorRotation(NewDirection.Rotation());

		//MoveForward
		FVector MoveVector =GetActorLocation() + GetActorForwardVector() * movementSpeed * DeltaSeconds;
		SetActorLocation(MoveVector);
	}
	else {
		APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PlayerUnit = Cast<APlayerUnit>(PC);
	}
}