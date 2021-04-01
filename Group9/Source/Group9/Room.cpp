// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "PlayerUnit.h"
#include "Door.h"
#include "Components/BoxComponent.h"
// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MyBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	MyBoxCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	//Somehow this only works when called in beginplay, not in constructor
	MyBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARoom::OnOverlap);
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoom::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if the otherActor is a player, then give the player itself as a current room
	/*if (OtherActor->IsA(APlayerUnit::StaticClass())) {
		UE_LOG(LogTemp, Log, TEXT("Player etered room"));
		Cast<APlayerUnit>(OtherActor)->GetRoom(this);
	}*/
}

