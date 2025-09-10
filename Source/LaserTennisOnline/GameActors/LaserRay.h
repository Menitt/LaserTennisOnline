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

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* MeshNiagara;

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioComponent;
	
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
	UPROPERTY(EditDefaultsOnly, Category = "Sound") 
	class USoundCue* HitSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float HitScaleVolume = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float HitScalePitch = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float HitStartTime = 0.2;
	UFUNCTION(NetMulticast, Reliable) void PlayHitSound();

	UPROPERTY(EditDefaultsOnly, Category = "Sound") 
	class USoundCue* ChargeSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ChargeScaleVolume = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ChargeScalePitch = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ChargeStartTime = 0.2;
	UFUNCTION(NetMulticast, Reliable) void PlayChargeSound();

// VFX
private:
	UPROPERTY(EditDefaultsOnly, Category = "VFX") class UNiagaraSystem* LaserSpawnVFXTemplate;

	UFUNCTION()
	void ActivateLaser(class UNiagaraComponent* FinishedSystem);


};
