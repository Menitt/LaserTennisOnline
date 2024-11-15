// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBelt.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MovingBeltObject.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMovingBelt::AMovingBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	// Components
	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Despawning BoX Component");
	RootComponent = Mesh;

	ObjectSpawnPoint = CreateDefaultSubobject<USceneComponent>("Object Spawn Point");
	
	ObjectSpawnPoint->SetupAttachment(RootComponent);

	
	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMovingBelt::BeginPlay()
{
	Super::BeginPlay();

	AMovingBeltObject* Object = Cast<AMovingBeltObject>(GetWorld()->SpawnActor<AActor>(MovingBeltObjectClass,
	ObjectSpawnPoint->GetComponentLocation(),ObjectSpawnPoint->GetComponentRotation()));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SpawnObject, 1.f, true);


	// Enable overlap events
    BoxComponent->SetGenerateOverlapEvents(true);
    
    // Set the collision response
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // For dynamic world actors
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AMovingBelt::OnOverlapBegin);
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
		
		int Index = FMath::RandRange(0, ObjectMeshArray.Num()-1);	
		
		UStaticMeshComponent* ObjectMesh = Cast<UStaticMeshComponent>(Object->GetRootComponent());
		if (ObjectMesh)
		{
			float ScaleFactor = 0.7;
    		UStaticMesh* NewMesh = ObjectMeshArray[Index];
			ObjectMesh->SetStaticMesh(NewMesh);
			ObjectMesh->SetWorldScale3D(ScaleFactor * FVector(1,1,1));
			ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			
		}
	}
}


void AMovingBelt::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
bool bFromSweep, const FHitResult& SweepResult)
{

	AMovingBeltObject* ObjectToDespawn = Cast<AMovingBeltObject>(OtherActor);

	if (ObjectToDespawn)
	{
		ObjectToDespawn->Destroy();
	}
}
