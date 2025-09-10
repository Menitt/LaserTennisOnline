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
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

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

	RootComponent = CollisionComponent;
	Mesh->SetupAttachment(RootComponent);
	
	ProjectileComp->UpdatedComponent = RootComponent;

	MeshNiagara = CreateDefaultSubobject<UNiagaraComponent>("Mesh VFX");
	MeshNiagara->SetupAttachment(Mesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(RootComponent);

}	

// Called when the game starts or when spawned
void ALaserRay::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHitObject);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapObject);
	}

	// Deactivate Mesh Niagara
	if (IsValid(MeshNiagara))
	{
		MeshNiagara->Deactivate();
	}

	// Deactivate AudioComponent
	if (IsValid(AudioComponent))
	{
		AudioComponent->Deactivate();
	}

	// LaserSpawnVFX
	UNiagaraComponent* LaserSpawnNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	GetWorld(),
	LaserSpawnVFXTemplate,
	GetActorLocation(),
	GetActorRotation(),
	FVector(1.f),
	true, // auto destroy
	true, // auto activate
	ENCPoolMethod::AutoRelease
	);

	// Bind StartLaser to OnFinished
	if (IsValid(LaserSpawnNiagara))
	{
		
		PlayChargeSound();

		LaserSpawnNiagara->OnSystemFinished.AddDynamic(this, &ALaserRay::ActivateLaser);
	
		// Set Speed to 0
		if (IsValid(ProjectileComp))
		{
			ProjectileComp->InitialSpeed = 0;
		}
		
		// Set Mesh Invisible
		if (IsValid(Mesh))
		{
			Mesh->SetVisibility(false);
		}
	}


}


void ALaserRay::ActivateLaser(UNiagaraComponent* FinishedSystem)
{
	// Set Mesh back to visible
	if(IsValid(Mesh))
	{
		Mesh->SetVisibility(true);
	}

	// Start Moving
	if (IsValid(ProjectileComp))
	{
		ProjectileComp->MaxSpeed = 400;
		ProjectileComp->Velocity *= 400;
	}

	// Activate Niagara Effect
	if (IsValid(MeshNiagara))
	{
		MeshNiagara->Activate();
	}
	
	if (IsValid(AudioComponent))
	{
		AudioComponent->Activate();
	}	
	

}


// Called every frame
void ALaserRay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserRay::OnHitObject(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	// Do nothing if not Server (~ let server decide)
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsValid(OtherActor) && (OtherActor != this) && IsValid(OtherComp))
		{
			ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);

			if (Player)
			{
				Player->CustomTakeDamage();
				
				PlayHitSound();

				Destroy();
			}
		}

	}

}


void ALaserRay::OnBeginOverlapObject(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsValid(OtherActor) and OtherActor != this and IsValid(OtherComp))
		{
			if (OtherComp->ComponentHasTag(*DespawnSide))
			{
				Destroy();
			}
		}
	}
}


void ALaserRay::PlayHitSound_Implementation()
{
	if (IsValid(HitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation(),HitScaleVolume,HitScalePitch,HitStartTime);
	}
}


void ALaserRay::PlayChargeSound_Implementation()
{
	if (IsValid(ChargeSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this,ChargeSound,GetActorLocation(),ChargeScaleVolume,ChargeScalePitch,ChargeStartTime);
	}
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