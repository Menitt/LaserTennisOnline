// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Build Components
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	laserSpawnLocation_left = CreateDefaultSubobject<USceneComponent>("Spawn Location Left");
	laserSpawnLocation_right = CreateDefaultSubobject<USceneComponent>("Spawn Location Right");
	laserSpawnLocation_up = CreateDefaultSubobject<USceneComponent>("Spawn Location Up");
	laserSpawnLocation_down = CreateDefaultSubobject<USceneComponent>("Spawn Location Down");

	// Setup attachment
	this->RootComponent = mesh;
	laserSpawnLocation_left->SetupAttachment(RootComponent);
	laserSpawnLocation_right->SetupAttachment(RootComponent);
	laserSpawnLocation_up->SetupAttachment(RootComponent);
	laserSpawnLocation_down->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

