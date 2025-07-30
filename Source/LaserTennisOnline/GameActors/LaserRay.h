// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserRay.generated.h"

UCLASS()
class LASERTENNISONLINE_API ALaserRay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserRay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
// Destructor
	virtual void Destroyed() override;


//
// Components
//
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UBoxComponent* CollisionComponent;

//
// Gameplay
//
// Bind to OnComponentHit delegate.
	UFUNCTION()
    void OnHitObject(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlapObject(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);


public:
	FVector GetLaserBox() const;
	float CalculateDistance(FVector Location) const;
	bool IsTargetAhead(FVector Location) const;

	int EnemyPlayer;
	FString DespawnSide;

//Sound
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound") FString HitSoundFile;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ScaleVolume = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ScalePitch = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float StartTime = 0.;

	UPROPERTY() class USoundCue* HitSound;
	FString SoundFolder = "/Game/Assets/Audio/";

	UFUNCTION(NetMulticast, Reliable) void PlaySound();


// VFX
private:
	UPROPERTY(EditDefaultsOnly, Category = "VFX") class UNiagaraSystem* LaserSpawnVFXTemplate;
	UPROPERTY(EditDefaultsOnly, Category = "VFX") class UNiagaraSystem* LaserMeshVFXTemplate;


	UFUNCTION()
	void ActivateLaser(class UNiagaraComponent* FinishedSystem);


};
