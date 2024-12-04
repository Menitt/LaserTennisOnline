// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CentralGenerator.generated.h"


// Declare a multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSignalMessangerDelegate, int, nPlayer, int, nGenerator);


UCLASS()
class LASERTENNISONLINE_API ACentralGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACentralGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Spawn1;
	UPROPERTY(EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Spawn2;
	UPROPERTY(EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Spawn3;
	UPROPERTY(EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Spawn4;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SignalClass;

	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath1_1;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath1_2;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath1_3;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath1_4;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath2_1;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath2_2;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath2_3;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> SignalPath2_4;
	
	FVector SignalSpawnLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FSignalMessangerDelegate OnSignalArrived;

	void BroadCastSignalArrived(int nPlayer, int nGenerator);

	void SendSignal(int nPlayer, int nGenerator);

private:
	void SendSignal(const TArray<FVector2D>& SignalPath, int nPlayer, int nGenerator);



};
