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
	FName PlayerTag;
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 100.f;

	// Game Mode Exists only on the Server
	class ALaserTennisGameModeBase* GameMode;
	void SendSpawnLaserRequest();

public:

	UFUNCTION(NetMulticast, Reliable)
	void Activate();

	UFUNCTION(NetMulticast, Reliable)
	void Deactivate();

	bool IsPlatformActive() const {return bIsReady;};
	bool IsPlatformResting() const {return bIsResting;};
	bool IsPlatformActivating() const {return bShouldActivate;};
	bool IsPlatformDeactivating() const {return bShouldDeactivate;};


private:
	// //
	// // Replication
	// //
	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Replicated Properties to syncronize platform movement
	bool bShouldActivate = false;
	bool bShouldDeactivate = false;
	bool bIsPlayerReset = true;
	bool bIsReady = true;
	bool bIsResting = false;

};
