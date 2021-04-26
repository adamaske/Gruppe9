// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemyUnit.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUnit.h"

ATestEnemyUnit::ATestEnemyUnit() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	TestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	TestMesh->SetupAttachment(RootComponent);
}
void ATestEnemyUnit::Tick(float DeltaSeconds) {
	Movement(DeltaSeconds);
}

void ATestEnemyUnit::Movement(float DeltaSeconds)
{
	//Move to player
	if (FVector::Distance(PlayerUnit->GetActorLocation(), GetActorLocation()) < 200.f) {
		return;
	}

	if (PlayerUnit) {
		FVector TempLocation = PlayerUnit->GetActorLocation();
		TempLocation.Z = GetActorLocation().Z;
		FVector NewDirection = TempLocation - GetActorLocation();
		NewDirection.Normalize();

		SetActorRotation(NewDirection.Rotation());

		//MoveForward
		FVector MoveVector = GetActorLocation() + GetActorForwardVector() * movementSpeed * DeltaSeconds;
		SetActorLocation(MoveVector);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No player unit"));
		APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PlayerUnit = Cast<APlayerUnit>(PC);
	}
}