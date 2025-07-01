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
	LaserSpawnLocation_1_North = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 North");
	LaserSpawnLocation_1_South = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 South");
	LaserSpawnLocation_1_West = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 West");
	LaserSpawnLocation_1_East = CreateDefaultSubobject<USceneComponent>("Spawn Location 1 East");

	LaserSpawnLocation_2_North = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 North");
	LaserSpawnLocation_2_South = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 South");
	LaserSpawnLocation_2_West = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 West");
	LaserSpawnLocation_2_East = CreateDefaultSubobject<USceneComponent>("Spawn Location 2 East");

	LaserSpawnLocation_1_North->SetupAttachment(RootComponent);
	LaserSpawnLocation_1_South->SetupAttachment(RootComponent);
	LaserSpawnLocation_1_West->SetupAttachment(RootComponent);
	LaserSpawnLocation_1_East->SetupAttachment(RootComponent);
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
	float X_West = X_North - X_Offset;
	float X_East = X_North + X_Offset;
	
	FVector LocationNorth1 = FVector(X_North, Y_North, Z);
	FVector LocationSouth1 = FVector(X_North, -Y_North, Z);
	FVector LocationWest1 = FVector(X_West, 0., Z);
	FVector LocationEast1 = FVector(X_East, 0., Z);
	FVector LocationNorth2 = FVector(-X_North, Y_North, Z);
	FVector LocationSouth2 = FVector(-X_North, -Y_North, Z);
	FVector LocationWest2 = FVector(-X_East, 0., Z);
	FVector LocationEast2 = FVector(-X_West, 0., Z);

	FRotator RotationNorth = FRotator(0., 90.,0.);
	FRotator RotationSouth = FRotator(0., -90.,0.);
	FRotator RotationWest = FRotator(0., 0., 0.);
	FRotator RotationEast = FRotator(0., 180., 0.);

	LaserSpawnLocation_1_North->SetRelativeLocation(LocationNorth1);
	LaserSpawnLocation_1_North->SetRelativeRotation(RotationNorth);

	LaserSpawnLocation_1_South->SetRelativeLocation(LocationSouth1);
	LaserSpawnLocation_1_South->SetRelativeRotation(RotationSouth);

	LaserSpawnLocation_1_West->SetRelativeLocation(LocationWest1);
	LaserSpawnLocation_1_North->SetRelativeRotation(RotationWest);

	LaserSpawnLocation_1_East->SetRelativeLocation(LocationEast1);
	LaserSpawnLocation_1_East->SetRelativeRotation(RotationEast);

	LaserSpawnLocation_2_North->SetRelativeLocation(LocationNorth2);
	LaserSpawnLocation_2_North->SetRelativeRotation(RotationNorth);

	LaserSpawnLocation_2_South->SetRelativeLocation(LocationSouth2);
	LaserSpawnLocation_2_South->SetRelativeRotation(RotationSouth);

	LaserSpawnLocation_2_West->SetRelativeLocation(LocationWest2);
	LaserSpawnLocation_2_West->SetRelativeRotation(RotationWest);

	LaserSpawnLocation_2_East->SetRelativeLocation(LocationEast2);
	LaserSpawnLocation_2_East->SetRelativeRotation(RotationEast);

}

// Called every frame
void ALaserSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserSpawnManager::SpawnLaser(int Player)
{
	
	int Side = FMath::RandRange(1, 4);
	
	int SpawnLocationID = Player*10 + Side;
	
	USceneComponent* LaserSpawnComponent = nullptr;

	switch (SpawnLocationID)
	{
	case 11:
		LaserSpawnComponent = LaserSpawnLocation_2_North;
		break;
	case 12:
		LaserSpawnComponent = LaserSpawnLocation_2_South;
		break;
	case 13:
		LaserSpawnComponent = LaserSpawnLocation_2_West;
		break;
	case 14:
		LaserSpawnComponent = LaserSpawnLocation_2_East;
		break;
	case 21:
		LaserSpawnComponent = LaserSpawnLocation_1_North;
		break;
	case 22:
		LaserSpawnComponent = LaserSpawnLocation_1_South;
		break;
	case 23:
		LaserSpawnComponent = LaserSpawnLocation_1_West;
		break;
	case 24:
		LaserSpawnComponent = LaserSpawnLocation_1_East;
		break;
	default:
		break;
	}

	if (IsValid(LaserSpawnComponent))
	{
		FVector SpawnLocation = LaserSpawnComponent->GetComponentLocation();
		FRotator SpawnRotation = LaserSpawnComponent->GetComponentRotation();
		
		// ALaserRay* Laser = GetWorld()->SpawnActor<ALaserRay>(LaserClass, SpawnLocation, SpawnRotation);
		AActor* Laser = GetWorld()->SpawnActor<AActor>(LaserClass, SpawnLocation, SpawnRotation);

		UE_LOG(LogTemp, Warning, TEXT("Spawning Laser, LaserSpawnLocation: %d"), SpawnLocationID);
		UE_LOG(LogTemp, Warning, TEXT("Component Rotation: %s"), *SpawnRotation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Actor Rotation: %s"), *Laser->GetActorRotation().ToString());

		if (IsValid(Laser))
		{
			FString PlayerTag = FString::FromInt(Player);
			Laser->Tags.Add(*PlayerTag);
		}
	}
}