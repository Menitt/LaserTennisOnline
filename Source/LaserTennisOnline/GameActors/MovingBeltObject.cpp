// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBeltObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"



// Sets default values
AMovingBeltObject::AMovingBeltObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");

	MovementComponent->InitialSpeed = 300.0f;
	MovementComponent->MaxSpeed = 300.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;


}

// Called when the game starts or when spawned
void AMovingBeltObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingBeltObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

