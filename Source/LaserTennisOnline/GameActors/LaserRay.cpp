// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserRay.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "BasePlayer.h"
#include "Engine/BlockingVolume.h"

// Sets default values
ALaserRay::ALaserRay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Replication
	bReplicates = true;

	// Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");	
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Component");
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("Collision Component");

	// Collision
	CollisionComponent->SetCollisionProfileName("BlockAll");
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
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

	// Do nothing if not Server (~ let server decide)
	if (GetLocalRole() == ROLE_Authority)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);

			if (Player)
			{

				Player->CustomTakeDamage();

				Destroy();
			}

			else
			{
				ABlockingVolume* Volume = Cast<ABlockingVolume>(OtherActor);

				if (Volume)
				{
					Destroy();
				}
			}	
		}

	
	}

}


void ALaserRay::Destroyed()
{

}