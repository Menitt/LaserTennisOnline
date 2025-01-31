// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBeltObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AMovingBeltObject::AMovingBeltObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Overlap Box Component");

	RootComponent = Mesh;

	BoxComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");

	MovementComponent->InitialSpeed = Speed;
	MovementComponent->MaxSpeed = Speed;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;

	// Enable overlap events
    BoxComponent->SetGenerateOverlapEvents(true);
    
    // Set the collision response
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // For dynamic world actors
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// BoxComponent->SetHiddenInGame(false);

}

// Called when the game starts or when spawned
void AMovingBeltObject::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->SetHiddenInGame(true);
}

// Called every frame
void AMovingBeltObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

