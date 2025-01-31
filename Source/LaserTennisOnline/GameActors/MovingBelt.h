// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBelt.generated.h"

UCLASS()
class LASERTENNISONLINE_API AMovingBelt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingBelt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


//
// Components
//
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* ObjectSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AMovingBeltObject> MovingBeltObjectClass;


	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComponent;

//
// Gameplay
//
	UPROPERTY(EditDefaultsOnly)
	TArray<class UStaticMesh*> ObjectMeshArray;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly)
	float SpawnTimeInterval = 2.5f;
	void SpawnObject(); 

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult);

};
