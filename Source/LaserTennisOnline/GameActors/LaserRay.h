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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UBoxComponent* CollisionComponent;

//
// Gameplay
//
// Bind to OnComponentHit delegate.
	UFUNCTION(Category="Projectile")
    void OnHitPlayer(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	FVector GetLaserBox() const;

	float CalculateDistance(FVector Location) const;

	bool IsTargetAhead(FVector Location) const;

//Sound
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound") FString HitSoundFile;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ScaleVolume = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ScalePitch = 1.;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float StartTime = 0.;

	class USoundCue* HitSound;
	FString SoundFolder = "/Game/Assets/Audio/";

	UFUNCTION(NetMulticast, Reliable) void PlaySound();


};
