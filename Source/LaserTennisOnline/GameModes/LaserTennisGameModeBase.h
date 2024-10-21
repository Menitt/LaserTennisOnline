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

virtual void PostLogin(APlayerController* NewPlayer) override;

virtual void Tick(float DeltaTime) override;

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

	TArray<int> ActiveLaserPlatforms1; 
	TArray<int> ActiveLaserPlatforms2;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> ClassPlayer1;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> ClassPlayer2;
	bool bIsPlayer1 = true;
//
// Setup Function
//
private:
	void SetupGame();

	void SetupTimer();
	FTimerHandle TimerHandle;

//
// Gameplay
//
public:
	
	void ManagePlatforms();

	void UpdateActivePlatformsList(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	void AdjustPlatforms(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	void ActivatePlatform(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	void DeactivatePlatform(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	int GetNumberPlatformsByKey(const TArray<int>& PlatformsMap, int key) const;

	void SpawnLaserRequest(FName PlayerTag);

	void GameOver();

	virtual void HandleMatchHasEnded();

	UPROPERTY(EditDefaultsOnly)
	int nActivePlatforms;

};
