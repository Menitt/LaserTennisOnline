// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spark.generated.h"



// Declare a multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSparkDelegate, int, Player, int, Side);

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
	class USplineComponent* Spline;

	int ActivePlayer;
	int SpawnSide;

	UPROPERTY(EditDefaultsOnly)
	float Period = 5.;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpawnSide(class USplineComponent* PlatformSpline, int Player, int Side);

	FSparkDelegate OnSparkArrived;

protected:

	UFUNCTION()
	void HandleProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();


};
