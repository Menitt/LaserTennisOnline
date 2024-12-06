// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"


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
	
}

// Called every frame
void ALaserGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserGenerator::SpawnLaser_Implementation()
{
	
	UWorld* World = GetWorld();
	if (World)
	{	
		World->SpawnActor<AActor>(LaserRayClass, LaserSpawnPoint->GetComponentLocation(), 
		LaserSpawnPoint->GetComponentRotation());
	}

}