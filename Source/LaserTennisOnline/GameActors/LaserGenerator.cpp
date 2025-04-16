// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ALaserGenerator::ALaserGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	// Components
	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	LaserSpawnPoint = CreateDefaultSubobject<USceneComponent>("Laser Spawn Point");

	LaserSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALaserGenerator::BeginPlay()
{
	Super::BeginPlay();

	FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	Sound = LoadObject<USoundCue>(nullptr, *SoundPath);

}

// Called every frame
void ALaserGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserGenerator::SpawnLaser()
{
	
	UWorld* World = GetWorld();
	if (World and GetLocalRole() == ROLE_Authority)
	{	
		AActor* Laser = World->SpawnActor<AActor>(LaserRayClass, LaserSpawnPoint->GetComponentLocation(), 
		LaserSpawnPoint->GetComponentRotation());

		if (Tags.Num()>0 and Laser)
		{
			Laser->Tags.Add(this->Tags[0]);
		}
		
		PlaySound();
		
	}

}

void ALaserGenerator::PlaySound_Implementation()
{
	if (Sound and Sound->IsValidLowLevel())
	{
		UGameplayStatics::PlaySoundAtLocation(this,Sound,GetActorLocation(),ScaleVolume,ScalePitch,StartTime);
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1010, 2.f, FColor::Red, TEXT("ALaserGenerator->PlaySound: Could not play sound due to bad sound file"));
	}
}