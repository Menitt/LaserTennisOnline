// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserSpawnManager.generated.h"

UCLASS()
class LASERTENNISONLINE_API ALaserSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	// Laser Spawn Location Components
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_1_North;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_1_South;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_1_West;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_1_East;

	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_2_North;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_2_South;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_2_West;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserSpawnLocation_2_East;


	float X_East = -150.;
	float X_West = -2018.;
	float Y_West = 8.;
	float X_North = -1095.5;
	float Y_North = -1000.;
	float Y_South = 1010.;
	float Z = 40.;

	// Laser Ray Class
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> LaserClass;

public:

	UFUNCTION()
	void SpawnLaser(int Player);

};
