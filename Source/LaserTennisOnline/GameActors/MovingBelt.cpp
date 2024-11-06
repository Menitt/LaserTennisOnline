// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBelt.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MovingBeltObject.h"

// Sets default values
AMovingBelt::AMovingBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	// Components
	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	ObjectSpawnPoint = CreateDefaultSubobject<USceneComponent>("Object Spawn Point");
	
	ObjectSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMovingBelt::BeginPlay()
{
	Super::BeginPlay();

	AMovingBeltObject* Object = Cast<AMovingBeltObject>(GetWorld()->SpawnActor<AActor>(MovingBeltObjectClass,
	ObjectSpawnPoint->GetComponentLocation(),ObjectSpawnPoint->GetComponentRotation()));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SpawnObject, 0.5f, false);

}

// Called every frame
void AMovingBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMovingBelt::SpawnObject()
{
	AMovingBeltObject* Object = Cast<AMovingBeltObject>(GetWorld()->SpawnActor<AActor>(MovingBeltObjectClass,
	ObjectSpawnPoint->GetComponentLocation(),ObjectSpawnPoint->GetComponentRotation()));

	if (Object and ObjectMeshArray.Num()>0)
	{
		UStaticMeshComponent* ObjectMesh = Cast<UStaticMeshComponent>(Object->GetRootComponent());
		if (ObjectMesh)
		{
			ObjectMesh->SetStaticMesh(ObjectMeshArray[0]);
			ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
