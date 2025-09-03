// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spark.generated.h"



// Declare a multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSparkDelegate, int, Player);

UCLASS()
class LASERTENNISONLINE_API ASpark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpark();

protected:
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* Niagara;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class UAudioComponent* AudioComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timeline
	UPROPERTY(EditAnywhere)
	class UTimelineComponent* Timeline;

	// Curve Float
	UPROPERTY(EditAnywhere)
	UCurveFloat* MovementCurve;

	// Platform Reference
	class USplineComponent* Spline;

	int ActivePlayer;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 5.;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpline(class USplineComponent* PlatformSpline);
	void SetActivePlayer(int Player);
	void SetSide(int Side);


	FSparkDelegate OnSparkArrived;

protected:

	UFUNCTION()
	void HandleProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();

	// Sound
	UPROPERTY(EditDefaultsOnly, Category = "Sound") FString SoundFile;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ScaleVolume = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float ScalePitch = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") float StartTime = 0.f;
	FString SoundFolder = "/Game/Assets/Audio/";



};
