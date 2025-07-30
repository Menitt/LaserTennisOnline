// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserSpawnManager.h"
#include "Components\SceneComponent.h"
#include "Components\StaticMeshComponent.h"
#include "LaserRay.h"


// Sets default values
ALaserSpawnManager::ALaserSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	// Create Components

	LaserSpawnLocation_1_South = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 South");
	LaserSpawnLocation_1_West = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 West");
	LaserSpawnLocation_1_East = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 East");
	LaserSpawnLocation_1_North = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 North");

	LaserSpawnLocation_2_North = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 North");
	LaserSpawnLocation_2_South = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 South");
	LaserSpawnLocation_2_West = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 West");
	LaserSpawnLocation_2_East = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 East");

	LaserSpawnLocation_1_South->SetupAttachment(RootComponent);
	LaserSpawnLocation_1_West->SetupAttachment(RootComponent);
	LaserSpawnLocation_1_East->SetupAttachment(RootComponent);
	LaserSpawnLocation_1_North->SetupAttachment(RootComponent);
	LaserSpawnLocation_2_North->SetupAttachment(RootComponent);
	LaserSpawnLocation_2_South->SetupAttachment(RootComponent);
	LaserSpawnLocation_2_West->SetupAttachment(RootComponent);
	LaserSpawnLocation_2_East->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALaserSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Relative Location of LaserSpawnLocationComponents
	float Y_East = Y_West;
	float X_South = X_North;
	
	FVector LocationNorth1 = FVector(X_North, Y_North, Z);
	FVector LocationSouth1 = FVector(X_South, Y_South, Z);
	FVector LocationWest1 = FVector(X_West, Y_West, Z);
	FVector LocationEast1 = FVector(X_East, Y_East, Z);
	FVector LocationNorth2 = FVector(-X_North, Y_North, Z);
	FVector LocationSouth2 = FVector(-X_South, Y_South, Z);
	FVector LocationWest2 = FVector(-X_East, Y_West, Z);
	FVector LocationEast2 = FVector(-X_West, Y_East, Z);

	FRotator RotationNorth = FRotator(0., 90.,0.);
	FRotator RotationSouth = FRotator(0., -90.,0.);
	FRotator RotationWest = FRotator(0., 0., 0.);
	FRotator RotationEast = FRotator(0., 180., 0.);

	LaserSpawnLocation_1_North->SetWorldLocation(LocationNorth1);
	LaserSpawnLocation_1_North->SetRelativeRotation(RotationNorth);

	LaserSpawnLocation_1_South->SetWorldLocation(LocationSouth1);
	LaserSpawnLocation_1_South->SetRelativeRotation(RotationSouth);

	LaserSpawnLocation_1_West->SetWorldLocation(LocationWest1);
	LaserSpawnLocation_1_North->SetRelativeRotation(RotationWest);

	LaserSpawnLocation_1_East->SetWorldLocation(LocationEast1);
	LaserSpawnLocation_1_East->SetRelativeRotation(RotationEast);

	LaserSpawnLocation_2_North->SetWorldLocation(LocationNorth2);
	LaserSpawnLocation_2_North->SetRelativeRotation(RotationNorth);

	LaserSpawnLocation_2_South->SetWorldLocation(LocationSouth2);
	LaserSpawnLocation_2_South->SetRelativeRotation(RotationSouth);

	LaserSpawnLocation_2_West->SetWorldLocation(LocationWest2);
	LaserSpawnLocation_2_West->SetRelativeRotation(RotationWest);

	LaserSpawnLocation_2_East->SetWorldLocation(LocationEast2);
	LaserSpawnLocation_2_East->SetRelativeRotation(RotationEast);

	LaserSpawnLocation_1_North->SetWorldLocation(LocationNorth1);
	LaserSpawnLocation_1_North->SetRelativeRotation(RotationNorth);


}

// Called every frame
void ALaserSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserSpawnManager::SpawnLaser(int ActivePlayer)
{
	
	int Side = FMath::RandRange(1, 4);

	int SpawnLocationID = ActivePlayer*10 + Side;
	
	USceneComponent* LaserSpawnComponent = nullptr;
	
	FString DespawnSideTag;

	switch (SpawnLocationID)
	{
	case 11:
		LaserSpawnComponent = LaserSpawnLocation_2_North;
		DespawnSideTag = "South";
		break;
	case 12:
		LaserSpawnComponent = LaserSpawnLocation_2_South;
		DespawnSideTag = "North";
		break;
	case 13:
		LaserSpawnComponent = LaserSpawnLocation_2_West;
		DespawnSideTag = "East";
		break;
	case 14:
		LaserSpawnComponent = LaserSpawnLocation_2_East;
		DespawnSideTag = "West";
		break;
	case 21:
		LaserSpawnComponent = LaserSpawnLocation_1_North;
		DespawnSideTag = "South";
		break;
	case 22:
		LaserSpawnComponent = LaserSpawnLocation_1_South;
		DespawnSideTag = "North";
		break;
	case 23:
		LaserSpawnComponent = LaserSpawnLocation_1_West;
		DespawnSideTag = "East";
		break;
	case 24:
		LaserSpawnComponent = LaserSpawnLocation_1_East;
		DespawnSideTag = "West";
		break;
	default:
		break;
	}

	if (IsValid(LaserSpawnComponent))
	{
		FVector SpawnLocation = LaserSpawnComponent->GetComponentLocation();
		FRotator SpawnRotation = LaserSpawnComponent->GetComponentRotation();
		
		// ALaserRay* Laser = GetWorld()->SpawnActor<ALaserRay>(LaserClass, SpawnLocation, SpawnRotation);
		ALaserRay* Laser = GetWorld()->SpawnActor<ALaserRay>(LaserClass, SpawnLocation, SpawnRotation);

		if (IsValid(Laser))
		{
			FString PlayerTag = FString::FromInt(ActivePlayer);
			Laser->Tags.Add(*PlayerTag);
			Laser->DespawnSide = DespawnSideTag;
			if (ActivePlayer == 1)
			{
				Laser->EnemyPlayer = 2;
			}
			else
			{
				Laser->EnemyPlayer = 1;
			}
		}
	}
}