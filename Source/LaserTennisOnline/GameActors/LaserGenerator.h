// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserRay.h"

#include "LaserGenerator.generated.h"


UCLASS()
class LASERTENNISONLINE_API ALaserGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserGenerator();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class USceneComponent* LaserSpawnPoint;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALaserRay> LaserRayClass;


public:
	void SpawnLaser();


// Sound
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	FString SoundFile;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float StartTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ScaleVolume = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ScalePitch = 1.f;

	FString SoundFolder = "/Game/Assets/Audio/";
	UPROPERTY() class USoundCue* Sound;
	
	UFUNCTION(NetMulticast, Reliable) void PlaySound();


};
