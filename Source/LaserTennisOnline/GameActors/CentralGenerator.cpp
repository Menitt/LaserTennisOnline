// Fill out your copyright notice in the Description page of Project Settings.


#include "CentralGenerator.h"
#include "Components/SceneComponent.h"
#include "GeneratorSignal.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ACentralGenerator::ACentralGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");

	RootComponent = Mesh;

	Spawn1 = CreateDefaultSubobject<USceneComponent>("Spawn 1");
	Spawn2 = CreateDefaultSubobject<USceneComponent>("Spawn 2");
	Spawn3 = CreateDefaultSubobject<USceneComponent>("Spawn 3");
	Spawn4 = CreateDefaultSubobject<USceneComponent>("Spawn 4");

	Spawn1->SetupAttachment(RootComponent);
	Spawn2->SetupAttachment(RootComponent);
	Spawn3->SetupAttachment(RootComponent);
	Spawn4->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ACentralGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACentralGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACentralGenerator::BroadCastSignalArrived(int nPlayer, int nGenerator)
{
	// Check if the delegate has any listeners
    if (OnSignalArrived.IsBound())
    {
        OnSignalArrived.Broadcast(nPlayer, nGenerator); // Trigger the delegate
	}
}


void ACentralGenerator::SendSignal(int nPlayer, int nGenerator)
{
	int PathID = 10 * nPlayer + nGenerator;

	switch (PathID)
	{
		case 11:
			SignalSpawnLocation = Spawn1->GetComponentLocation();
			SendSignal(SignalPath1_1, nPlayer, nGenerator);
			break;
		
		case 12:
			SignalSpawnLocation = Spawn2->GetComponentLocation();
			SendSignal(SignalPath1_2, nPlayer, nGenerator);
			break;
		
		case 13:
			SignalSpawnLocation = Spawn2->GetComponentLocation();
			SendSignal(SignalPath1_3, nPlayer, nGenerator);
			break;

		case 14:
			SignalSpawnLocation = Spawn4->GetComponentLocation();
			SendSignal(SignalPath1_4, nPlayer, nGenerator);
			break;
		
		case 21:
			SignalSpawnLocation = Spawn1->GetComponentLocation();
			SendSignal(SignalPath2_1, nPlayer, nGenerator);	
			break;
		
		case 22:
			SignalSpawnLocation = Spawn3->GetComponentLocation();
			SendSignal(SignalPath2_2, nPlayer, nGenerator);
			break;
		
		case 23:
			SignalSpawnLocation = Spawn3->GetComponentLocation();
			SendSignal(SignalPath2_3, nPlayer, nGenerator);
			break;
		
		case 24:
			SignalSpawnLocation = Spawn4->GetComponentLocation();
			SendSignal(SignalPath2_4, nPlayer, nGenerator);
			break;

		default:
			break;
	}
}


void ACentralGenerator::SendSignal(const TArray<FVector2D>& SignalPath, int nPlayer, int nGenerator)
{
	
	AGeneratorSignal* GeneratorSignal = Cast<AGeneratorSignal>(GetWorld()->SpawnActor<AActor>(
		SignalClass,SignalSpawnLocation,GetActorRotation()));



	if (GeneratorSignal)
	{
		GeneratorSignal->SetPath(SignalPath, nPlayer, nGenerator);

		GeneratorSignal->SetCentralGenerator(this);
	}
}