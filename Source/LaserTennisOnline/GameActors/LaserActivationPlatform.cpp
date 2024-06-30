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

	if (bShouldMove)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),InitialLocation-FVector(0.,0.,zTargetOffset),
		DeltaTime,DeactivationMovementOffset/DeactivationTime*2));
	}

	//
	// Debug
	// 
	DrawDebugBox(GetWorld(), overlappingComp->GetComponentLocation(), 
	overlappingComp->GetScaledBoxExtent(), FColor::Blue, false,-1);

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
			UE_LOG(LogTemp, Warning, TEXT("End Overlap"));
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
			UE_LOG(LogTemp, Warning, TEXT("Start Overlap"));
		
			bIsPlayerReset = false;
			bIsReady = false;
			zTargetOffset = DeactivationMovementOffset;
			bShouldMove = true;
			FTimerHandle DummyHandle;
			GetWorldTimerManager().SetTimer(DummyHandle, this, &ThisClass::ResetPlatform, DeactivationTime/2, false);

			// Send Spawn Laser Request
			SendSpawnLaserRequest();

		}
	}
}

void ALaserActivationPlatform::ResetPlatform()
{
	zTargetOffset = 0;
	FTimerHandle DummyHandle;
	GetWorldTimerManager().SetTimer(DummyHandle, this, &ThisClass::StopMovement, DeactivationTime/2+0.1f, false);
}

void ALaserActivationPlatform::StopMovement()
{
	bShouldMove = false;
	bIsReady = true;
}


void ALaserActivationPlatform::SendSpawnLaserRequest()
{
	if (GameMode)
	{
		GameMode->SpawnLaserRequest(PlayerTag);
	}
}

void ALaserActivationPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALaserActivationPlatform, bShouldMove);
	DOREPLIFETIME(ALaserActivationPlatform, bIsReady);
	DOREPLIFETIME(ALaserActivationPlatform, bIsPlayerReset);
	DOREPLIFETIME(ALaserActivationPlatform, zTargetOffset);
}

void ALaserActivationPlatform::OnRep_ShouldMove()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Executing OnRep_ShouldMove");
	}
}