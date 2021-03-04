// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableUnit.h"
#include "PlayerUnit.h"
#include "Components/SphereComponent.h"

// Sets default values
AInteractableUnit::AInteractableUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	InteractableMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshComponent"));
	RootComponent = InteractableMeshComponent;
	//Adds OnOverlap begin and ended
	MyCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SomeCollisionComponent"));
	MyCollisionComponent->SetupAttachment(RootComponent);
	MyCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableUnit::BeganOverlap);
	MyCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractableUnit::EndedOverlap);
}

// Called when the game starts or when spawned
void AInteractableUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Should check if the player is within range
}

void AInteractableUnit::InteractWithPlayer(APlayerUnit* player) 
{
	//Called when the player presses the interact key
	UE_LOG(LogTemp, Error, TEXT("Player interacted with me"));
}


void AInteractableUnit::BeganOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Checks if this is a PlayerUnit
	if (OtherActor->IsA(APlayerUnit::StaticClass())) {
		UE_LOG(LogTemp, Error, TEXT("Started collision with player"));
		//Calls to give this as the new interavctle to the player
		Cast<APlayerUnit>(OtherActor)->GetInteractableUnit(this);
	}
}

void AInteractableUnit::EndedOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Checks if this is a PlayerUnit
	if (OtherActor->IsA(APlayerUnit::StaticClass())) {
		UE_LOG(LogTemp, Error, TEXT("Ended collision with player"));
		//Calls to remove this from the player
		Cast<APlayerUnit>(OtherActor)->RemoveInteractableUnit(this);
	}
}
