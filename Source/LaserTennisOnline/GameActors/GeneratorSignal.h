// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorSignal.generated.h"

UCLASS()
class LASERTENNISONLINE_API AGeneratorSignal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneratorSignal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void SetPath(const TArray<FVector2D>& NavigationPath, int nPlayer, int nGenerator);

	void SetCentralGenerator(class ACentralGenerator* Generator);

private:	

	
	int Player;
	int Generator;
	
	class ACentralGenerator* CentralGenerator;

	TArray<FVector2D> Path;

	int StopCounter = 0;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 10;


};
