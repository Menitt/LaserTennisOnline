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

//
// Gameplay
//
UFUNCTION()
void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

UFUNCTION()
void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

void ResetPlatform();
void StopMovement();

float DeactivationTime = 2.0f;


FVector InitialLocation;

float DeactivationMovementOffset = 100;

// Game Mode Exists only on the Server
class ALaserTennisGameModeBase* GameMode;

// Replicated Properties to syncronize platform movement
UPROPERTY(Replicated) bool bShouldMove = false;
UPROPERTY(ReplicatedUsing = OnRep_ShouldMove) bool bIsPlayerReset = true;
UPROPERTY(Replicated) bool bIsReady = true;
UPROPERTY(Replicated) float zTargetOffset = 0.f;
virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

UFUNCTION()
void OnRep_ShouldMove();


FName PlayerTag;
void SendSpawnLaserRequest();



};
