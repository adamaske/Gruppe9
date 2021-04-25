// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemyOnCollision.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "Components/SphereComponent.h"
// Sets default values
ASpawnEnemyOnCollision::ASpawnEnemyOnCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	MyCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	EnemySpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));

	MyCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpawnEnemyOnCollision::BeganOverlap);
}

// Called when the game starts or when spawned
void ASpawnEnemyOnCollision::BeginPlay()
{
	Super::BeginPlay();
	//Just destroy itself if there is no blueprint specified
	if (!EnemyBP) {
		this->Destroy();
	}
}

// Called every frame
void ASpawnEnemyOnCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnEnemyOnCollision::BeganOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerUnit::StaticClass()) && !bHasSpawned) {
		Spawn(Cast<APlayerUnit>(OtherActor));
	}
}

void ASpawnEnemyOnCollision::EndedOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerUnit::StaticClass())) {

	}
}

void ASpawnEnemyOnCollision::Spawn(APlayerUnit* player) {
	bHasSpawned = true;

	FVector Pos = EnemySpawnPoint->GetComponentLocation();
	UWorld* world = GetWorld();

	if (world) {
		AEnemyUnit* unit = world->SpawnActor<AEnemyUnit>(EnemyBP, Pos, GetActorRotation());
		unit->PlayerUnit = player;
		unit->bDestroyMyselfOnDeath = true;
	}
	
	//Destroy itself
	this->Destroy();
}
