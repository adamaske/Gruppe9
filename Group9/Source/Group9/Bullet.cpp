// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/SphereComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Set a empty object to root
	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));

	meshComponent->SetupAttachment(RootComponent);

	Cast<USphereComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(DeltaTime);
}

void ABullet::MoveForward(float DeltaTime)
{
	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * ProjectileSpeed * DeltaTime;
	SetActorLocation(newLocation);
}

void ABullet::Lifetime(float DeltaTime)
{
	//Adds time to timeLIved
	timeLived += DeltaTime;
	//Checks if its above or the same as allowed
	if (timeLived >= allowedLifetime) {
		this->Destroy();
	}
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Bullet hit something"));
}

