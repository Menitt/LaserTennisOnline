// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorSignal.h"
#include "CentralGenerator.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AGeneratorSignal::AGeneratorSignal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio Component");

	RootComponent = MeshComponent;

	AudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGeneratorSignal::BeginPlay()
{
	Super::BeginPlay();

	FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	USoundWave* Sound = LoadObject<USoundWave>(nullptr, *SoundPath);  

	if (AudioComponent and Sound and Sound->IsValidLowLevel())
	{
		AudioComponent->SetSound(Sound);
		AudioComponent->Play();
	}

}

// Called every frame
void AGeneratorSignal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector InitialLocation = GetActorLocation();

	if (Path.Num()>StopCounter and (FVector2D(GetActorLocation().X, GetActorLocation().Y)-Path[StopCounter]).Size()<100)
	{
		SetActorLocation(FVector(Path[StopCounter].X, Path[StopCounter].Y, GetActorLocation().Z));
		StopCounter += 1;
	}

	if (StopCounter != 0 and StopCounter == Path.Num())
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			if (CentralGenerator)
			{
				CentralGenerator->BroadCastSignalArrived(Player, Generator);
			}
			Destroy();
		}
	}

	if (Path.Num()>StopCounter)
	{
		FVector TargetLocation = FVector(Path[StopCounter].X, Path[StopCounter].Y, GetActorLocation().Z);

		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();

		// FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);

		FVector NewLocation = Direction * DeltaTime * Speed * 100 + GetActorLocation();

		SetActorLocation(NewLocation);
	}



}

void AGeneratorSignal::SetPath_Implementation(const TArray<FVector2D>& NavigationPath, int nPlayer, int nGenerator)
{
	Path = NavigationPath;
	Player = nPlayer;
	Generator = nGenerator;
}

void AGeneratorSignal::SetCentralGenerator(ACentralGenerator* GeneratorPtr)
{
	CentralGenerator = GeneratorPtr;
}

void AGeneratorSignal::Destroyed()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
	
	Super::Destroyed();
}