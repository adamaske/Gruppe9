// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"


UCLASS()
class GROUP9_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();
	UPROPERTY(EditAnywhere)
	TArray<ARoom*> MyConnectedRooms;
	UPROPERTY(EditAnywhere)
	TArray<class ADoor*> ConnectedDoors;
	TArray<ARoom*> AvailableRooms;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* MyBoxCollision;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateConnectedRooms();

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
};
