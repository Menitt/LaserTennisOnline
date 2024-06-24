// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserRay.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "BasePlayer.h"

// Sets default values
ALaserRay::ALaserRay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");	
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Component");
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("Collision Component");

	// Collision
	CollisionComponent->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = CollisionComponent;

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHitPlayer);
	}

	Mesh->SetupAttachment(RootComponent);

	ProjectileComp->SetUpdatedComponent(CollisionComponent);
	ProjectileComp->InitialSpeed = 300.0f;
	ProjectileComp->MaxSpeed = 300.0f;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void ALaserRay::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserRay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserRay::OnHitPlayer(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);

	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserRay->OnHitPlayer"));

		Player->CustomTakeDamage();

		Destroy();
	}

}

void ALaserRay::Destroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("ALaserRay->Destroyed"));
}