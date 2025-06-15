// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserActivationPlatform.generated.h"

UCLASS()
class LASERTENNISONLINE_API ALaserActivationPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserActivationPlatform();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* baseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* movingMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* overlappingComp;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* Spline;

//
// Gameplay
//
private:
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FVector InitialLocation;
	FVector RestingLocation;
	FVector DeactivationMovementOffset = FVector(0.,0.,100.);
	
	UPROPERTY(EditDefaultsOnly)
	int OtherPlayerID;
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 100.f;

	// Game Mode Exists only on the Server
	class ALaserTennisGameModeBase* GameMode;
	void SendSpawnLaserRequest();


	UPROPERTY(EditDefaultsOnly, Category = "Spline")
	TSubclassOf<AActor> SparkClass;


public:

	// UFUNCTION(NetMulticast, Reliable)
	void Activate();

	// UFUNCTION(NetMulticast, Reliable)
	void Deactivate();

	bool IsPlatformActive() const {return bIsReady;};
	bool IsPlatformResting() const {return bIsResting;};
	bool IsPlatformActivating() const {return bShouldActivate;};
	bool IsPlatformDeactivating() const {return bShouldDeactivate;};


private:
	//
	// Replication
	//
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Replicated Properties to syncronize platform movement
	UPROPERTY(Replicated)
	bool bShouldActivate = false;
	UPROPERTY(Replicated)
	bool bShouldDeactivate = false;
	UPROPERTY(Replicated)
	bool bIsPlayerReset = true;
	UPROPERTY(Replicated)
	bool bIsReady = true;
	UPROPERTY(Replicated)
	bool bIsResting = false;

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
	void PlayUISound();

	UFUNCTION(NetMulticast, Reliable)
	void PlaySoundEffect();
};
