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
#include "Sound/SoundCue.h"
#include "Components\SplineComponent.h"
#include "Spark.h"
#include "Kismet/KismetMathLibrary.h"
#include "LaserSpawnManager.h"

// Sets default values
ALaserActivationPlatform::ALaserActivationPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// Components
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	movingMesh = CreateDefaultSubobject<UStaticMeshComponent>("Moving Mesh");
	RootComponent = baseMesh;
	movingMesh->SetupAttachment(baseMesh);
	overlappingComp = CreateDefaultSubobject<UBoxComponent>("Overlapping Component");
	overlappingComp->SetupAttachment(movingMesh);

	// Spline Component
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(baseMesh);

}

// Called when the game starts or when spawned
void ALaserActivationPlatform::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;

	// Run on all Clients
	if (overlappingComp)
	{
		InitialLocation = movingMesh->GetComponentLocation();
		RestingLocation = InitialLocation - DeactivationMovementOffset;
		overlappingComp->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBeginOverlap);
		overlappingComp->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnEndOverlap);
	}

	// Run only on Server
	if (GetLocalRole() == ROLE_Authority)
	{
		GameMode = Cast<ALaserTennisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	// Try to Fetch LaserSpawnManager Reference (not reliable to be already initialized -> need redundancy)
	FetchLaserSpawnManager();

	// Bind Sound File
	FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	Sound = LoadObject<USoundCue>(nullptr, *SoundPath);

}

void ALaserActivationPlatform::FetchLaserSpawnManager()
{
	TArray<AActor*> TempArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaserSpawnManager::StaticClass(), TempArray);

	if (TempArray.Num() > 0)
	{
		LaserSpawnManager = Cast<ALaserSpawnManager>(TempArray[0]);
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
		movingMesh->SetWorldLocation(FMath::VInterpConstantTo(movingMesh->GetComponentLocation(),InitialLocation,
		DeltaTime,MovementSpeed));
	}

	//
	// Deactivation
	//
	if (bShouldDeactivate)
	{
		movingMesh->SetWorldLocation(FMath::VInterpConstantTo(movingMesh->GetComponentLocation(),RestingLocation,
		DeltaTime,MovementSpeed));
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		if (movingMesh->GetComponentLocation() == InitialLocation and bIsPlayerReset)
		{
			bIsReady = true;
			bShouldActivate = false;
		}
		
		if (movingMesh->GetComponentLocation() == RestingLocation)
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

			PlaySoundEffect();
			
			// Deactivate
			Deactivate();
		}
	}
}

void ALaserActivationPlatform::SendSpawnLaserRequest()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		// Spawn Spark Actor
		ASpark* Spark = GetWorld()->SpawnActor<ASpark>(SparkClass, baseMesh->GetComponentLocation(), baseMesh->GetComponentRotation());
		if (IsValid(Spark))
		{
			
			Spark->SetSpline(Spline);
			Spark->SetActivePlayer(ActivePlayer);
			
			if (not IsValid(LaserSpawnManager))
			{
				FetchLaserSpawnManager();
			}
			if (IsValid(LaserSpawnManager))
			{
				Spark->OnSparkArrived.AddDynamic(LaserSpawnManager, &ALaserSpawnManager::SpawnLaser);
			}
	
		}
	}	
}

void ALaserActivationPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALaserActivationPlatform, bShouldActivate);
	DOREPLIFETIME(ALaserActivationPlatform, bShouldDeactivate);
	DOREPLIFETIME(ALaserActivationPlatform, bIsReady);
	DOREPLIFETIME(ALaserActivationPlatform, bIsPlayerReset);
	DOREPLIFETIME(ALaserActivationPlatform, bIsResting);
}

void ALaserActivationPlatform::Deactivate()
{
	if (GetLocalRole() == ROLE_Authority)
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
}

void ALaserActivationPlatform::Activate()
{
	
	if (GetLocalRole() == ROLE_Authority)
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
}

void ALaserActivationPlatform::PlaySoundEffect_Implementation()
{
	if (Sound and Sound->IsValidLowLevel())
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation(), ScaleVolume, ScalePitch, StartTime);
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1010, 2.f, FColor::Red, TEXT("LaserActivationPlatform->PlaySoundEffec: Could not play sound due to bad sound file"));
	}
}