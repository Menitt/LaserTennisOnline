// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LaserGenerator.h"
#include "LaserActivationPlatform.h"

#include "LaserTennisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ALaserTennisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

//
// Gameplay Actors Containers
//
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> laserGeneratorClass;
	TArray<AActor*> laserGenerators1;
	TArray<AActor*> laserGenerators2;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> laserActivationPlatformClass;
	TArray<AActor*> laserPlatforms1; 
	TArray<AActor*> laserPlatforms2;

//
// Setup Function
//
private:
	void SetupGame();

};
