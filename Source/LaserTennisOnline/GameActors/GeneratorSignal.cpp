// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorSignal.h"
#include "CentralGenerator.h"

// Sets default values
AGeneratorSignal::AGeneratorSignal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeneratorSignal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGeneratorSignal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Path.Num()>StopCounter and (FVector2D(GetActorLocation().X, GetActorLocation().Y)-Path[StopCounter]).Size()<10)
	{
		StopCounter += 1;
	}

	if (StopCounter != 0 and StopCounter == Path.Num())
	{
		if (CentralGenerator)
		{
			CentralGenerator->BroadCastSignalArrived(Player, Generator);
		}
		Destroy();
	}

	if (Path.Num()>StopCounter)
	{
		FVector TargetLocation = FVector(Path[StopCounter].X, Path[StopCounter].Y, GetActorLocation().Z);

		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
	
		SetActorLocation(NewLocation);
	}

}

void AGeneratorSignal::SetPath(TArray<FVector2D> NavigationPath, int nPlayer, int nGenerator)
{
	Path = NavigationPath;
	Player = nPlayer;
	Generator = nGenerator;
}

void AGeneratorSignal::SetCentralGenerator(ACentralGenerator* GeneratorPtr)
{
	CentralGenerator = GeneratorPtr;
}