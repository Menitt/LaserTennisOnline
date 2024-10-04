// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LaserGenerator.h"
#include "LaserActivationPlatform.h"

#include "LaserTennisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ALaserTennisGameModeBase : public AGameMode
{
	GENERATED_BODY()


virtual void BeginPlay() override;



//
// Gameplay Actors Containers
//
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ALaserGenerator> laserGeneratorClass;
	TArray<AActor*> laserGenerators1;
	TArray<AActor*> laserGenerators2;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ALaserActivationPlatform> laserActivationPlatformClass;
	TArray<AActor*> laserPlatforms1; 
	TArray<AActor*> laserPlatforms2;

//
// Setup Function
//
private:
	void SetupGame();


//
// Gameplay
//
public:
	void SpawnLaserRequest(FName PlayerTag);

	void GameOver();

	virtual void HandleMatchHasEnded();


};
