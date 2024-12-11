// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStartPanel.generated.h"

// Declare a multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSignalGameStartDelegate);

UCLASS()
class LASERTENNISONLINE_API AGameStartPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameStartPanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* GameStartWidget;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 50;
	FVector TargetLocation;
	bool bShouldMove = false;

public:

	UFUNCTION(NetMulticast, Reliable)
	void StartCountdown();

	FSignalGameStartDelegate OnGameStarting;

	void BroadcastGameStart();

};
 