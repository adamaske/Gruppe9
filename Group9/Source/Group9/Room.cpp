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

void ARoom::UpdateConnectedRooms()
{
	//Clears the array first
	AvailableRooms.Empty();

	//For each of my connected rooms, if their rooms is not already in the array, add them to it
	for (int i{ 0 }; i < MyConnectedRooms.Num(); i++) {
		for (int j{ 0 }; j < MyConnectedRooms[i]->MyConnectedRooms.Num(); j++)
		{
			if (!AvailableRooms.Contains(MyConnectedRooms[i]->MyConnectedRooms[j])) {
				AvailableRooms.Add(MyConnectedRooms[i]->MyConnectedRooms[j]);
			}
		}
	}
	//CHeck each of my connectted doors, Check if they are open, if they are add their rooms to avalible rooms
	for (int i{ 0 }; i < ConnectedDoors.Num(); i++) {
		if (ConnectedDoors[i]->bIsOpen) {
			for (int j{ 0 }; j < ConnectedDoors[i]->RoomsBehindMe.Num(); j++)
			{
				AvailableRooms.Add(ConnectedDoors[i]->RoomsBehindMe[j]);
			}
		}
	}
}

void ARoom::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if the otherActor is a player, then give the player itself as a current room
	if (OtherActor->IsA(APlayerUnit::StaticClass())) {
		UE_LOG(LogTemp, Log, TEXT("Player etered room"));
		Cast<APlayerUnit>(OtherActor)->GetRoom(this);
	}
}

