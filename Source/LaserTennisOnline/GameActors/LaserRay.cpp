// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserRay.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "BasePlayer.h"
#include "Engine/BlockingVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"

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
	
	RootComponent = Mesh;
	CollisionComponent->SetupAttachment(RootComponent);
	ProjectileComp->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void ALaserRay::BeginPlay()
{
	Super::BeginPlay();
	
	FString HitSoundPath = SoundFolder + HitSoundFile + "." + HitSoundFile;
	HitSound = LoadObject<USoundCue>(nullptr, *HitSoundPath);

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHitPlayer);
	}
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
				
				PlaySound();

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

void ALaserRay::PlaySound_Implementation()
{
	if (HitSound and HitSound->IsValidLowLevel())
	{
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation(),ScaleVolume,ScalePitch,StartTime);
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1010, 2.f, FColor::Red, TEXT("ALaserRay->PlaySound: Could not play sound due to bad sound file"));
	}
}

void ALaserRay::Destroyed()
{
	Super::Destroyed();
}

FVector ALaserRay::GetLaserBox() const
{
	if (CollisionComponent)
	{
		return CollisionComponent->GetScaledBoxExtent();
	}
	else
	{
		return FVector(0,0,0);
	}
}

float ALaserRay::CalculateDistance(FVector Location) const 
{
	FTransform WorldTransform = this->GetActorTransform();

	FVector LocalLocation = WorldTransform.InverseTransformVector(Location-this->GetActorLocation());

	FVector LocationProjection = WorldTransform.TransformVector(FVector(0,LocalLocation.Y,0)) + GetActorLocation(); 

	return (LocationProjection - Location).Size();
}

bool ALaserRay::IsTargetAhead(FVector Location) const
{
	FTransform WorldTransform = this->GetActorTransform();

	FVector LocalLocation = WorldTransform.InverseTransformVector(Location-this->GetActorLocation());

	return LocalLocation.X > 0;
}