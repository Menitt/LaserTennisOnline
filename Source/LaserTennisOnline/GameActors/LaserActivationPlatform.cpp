// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserActivationPlatform.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ActorComponent.h"
#include "Components\BoxComponent.h"
#include "BasePlayer.h"
#include "Kismet\GameplayStatics.h"
#include "LaserTennisGameModeBase.h"
#include "DrawDebugHelpers.h"


// Sets default values
ALaserActivationPlatform::ALaserActivationPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	// Bind Function to Delegate and Setup Movement target
	if (overlappingComp)
	{
		overlappingComp->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBeginOverlap);
		overlappingComp->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnEndOverlap);
		InitialLocation = GetActorLocation();
	}
	
	GameMode = Cast<ALaserTennisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

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
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),Target,
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
	ABasePlayer* player = Cast<ABasePlayer>(OtherActor);
	
	if (player and !bIsPlayerReset and !bIsReady)
	{
		bIsPlayerReset = true;
		UE_LOG(LogTemp, Warning, TEXT("End Overlap"));
	}
}


void ALaserActivationPlatform::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, 
AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ABasePlayer* player = Cast<ABasePlayer>(OtherActor);
	
	if (player and bIsPlayerReset and bIsReady)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Overlap"));
	
		bIsPlayerReset = false;
		bIsReady = false;
		Target = InitialLocation - FVector::UpVector*DeactivationMovementOffset;
		bShouldMove = true;
		FTimerHandle DummyHandle;
		GetWorldTimerManager().SetTimer(DummyHandle, this, &ThisClass::ResetPlatform, DeactivationTime/2, false);

		// Send Spawn Laser Request
		SendSpawnLaserRequest();

	}
}

void ALaserActivationPlatform::ResetPlatform()
{
	Target = InitialLocation;
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