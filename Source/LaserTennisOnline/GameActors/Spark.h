// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spark.generated.h"

UCLASS()
class LASERTENNISONLINE_API ASpark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh

	// UPROPERTY(EditDefaultsOnly)
	// class UStaticMeshComponent* Mesh;

	// Timeline
	UPROPERTY(EditAnywhere)
	class UTimelineComponent* Timeline;

	// Curve Float
	UPROPERTY(EditAnywhere)
	UCurveFloat* MovementCurve;

	// Platform Reference
	class ALaserActivationPlatform* PlatformOwner;

	UPROPERTY(EditDefaultsOnly)
	float Period = 5.;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlatformOwner(class ALaserActivationPlatform* Platform);

protected:

	UFUNCTION()
	void HandleProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();


};
