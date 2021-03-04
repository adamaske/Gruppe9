// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableUnit.generated.h"

UCLASS()
class GROUP9_API AInteractableUnit : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* InteractableMeshComponent;

	UPROPERTY(EditAnywhere)
		class USphereComponent* MyCollisionComponent;
public:	
	// Sets default values for this actor's properties
	AInteractableUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InteractWithPlayer(class APlayerUnit*);

	UFUNCTION()
		void BeganOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndedOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
