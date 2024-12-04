// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserActivationPlatform.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ActorComponent.h"
#include "Components\BoxComponent.h"
#include "BasePlayer.h"
#include "Kismet\GameplayStatics.h"
#include "LaserTennisGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALaserActivationPlatform::ALaserActivationPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// Components
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	movingMesh = CreateDefaultSubobject<UStaticMeshComponent>("Moving Mesh");
	RootComponent = movingMesh;
	baseMesh->SetupAttachment(RootComponent);
	overlappingComp = CreateDefaultSubobject<UBoxComponent>("Overlapping Component");
	overlappingComp->SetupAttachment(movingMesh);

}

// Called when the game starts or when spawned
void ALaserActivationPlatform::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;

	// Run on all Clients
	if (overlappingComp)
	{
		InitialLocation = GetActorLocation();
		RestingLocation = InitialLocation - DeactivationMovementOffset;
		overlappingComp->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBeginOverlap);
		overlappingComp->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnEndOverlap);
	}

	// Run only on Server
	if (GetLocalRole() == ROLE_Authority)
	{
		GameMode = Cast<ALaserTennisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	
	baseMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (Tags.Num() > 0) 
	{
		PlayerTag = Tags[0];
	}

}

// Called every frame
void ALaserActivationPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 
	// Activation
	//
	if (bShouldActivate)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),InitialLocation,
		DeltaTime,MovementSpeed));
	}

	//
	// Deactivation
	//
	if (bShouldDeactivate)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),RestingLocation,
		DeltaTime,MovementSpeed));
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		if (GetActorLocation() == InitialLocation and bIsPlayerReset)
		{
			bIsReady = true;
			bShouldActivate = false;
		}
		
		if (GetActorLocation() == RestingLocation)
		{
			bShouldDeactivate = false;
			bIsResting = true;

		}
	}

}

void ALaserActivationPlatform::OnEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	// Do nothing if not on Server

	if (GetLocalRole() == ROLE_Authority)
	{
		ABasePlayer* player = Cast<ABasePlayer>(OtherActor);

		if (player and !bIsPlayerReset and !bIsReady)
		{
			bIsPlayerReset = true;
		}
	}
}


void ALaserActivationPlatform::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, 
AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	
	// Do nothing if not on Server

	if (GetLocalRole() == ROLE_Authority)
	{
		ABasePlayer* player = Cast<ABasePlayer>(OtherActor);
		
		if (player and bIsPlayerReset and bIsReady)
		{
		
			bIsPlayerReset = false;
		
			// Send Spawn Laser Request
			SendSpawnLaserRequest();

			// Deactivate
			Deactivate();

		}
	}
}

void ALaserActivationPlatform::SendSpawnLaserRequest()
{
	if (GameMode)
	{
		GameMode->SpawnLaserRequest(PlayerTag);
	}
}

// void ALaserActivationPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

// 	DOREPLIFETIME(ALaserActivationPlatform, bShouldActivate);
// 	DOREPLIFETIME(ALaserActivationPlatform, bShouldDeactivate);
// 	DOREPLIFETIME(ALaserActivationPlatform, bIsReady);
// 	DOREPLIFETIME(ALaserActivationPlatform, bIsPlayerReset);
// 	DOREPLIFETIME(ALaserActivationPlatform, bIsResting);
// }

void ALaserActivationPlatform::Deactivate_Implementation()
{
	if (bShouldActivate or bShouldDeactivate)
	{
		return;
	}

	bIsReady = false;
	bShouldDeactivate = true;
	bShouldActivate = false;
	bIsResting = false;

}

void ALaserActivationPlatform::Activate_Implementation()
{
	if (bShouldActivate or bShouldDeactivate)
	{
		return;
	}

	bIsReady = false;
	bShouldDeactivate = false;
	bShouldActivate = true;
	bIsResting = false;

}

