// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LaserGenerator.h"
#include "LaserActivationPlatform.h"
#include "CentralGenerator.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ACentralGenerator> CentralGeneratorClass;
	ACentralGenerator* CentralGenerator;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> ClassPlayer1;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> ClassPlayer2;
	bool bIsPlayer1 = true;

	class ALaserGenerator* LaserGenerator1_1;
	class ALaserGenerator* LaserGenerator1_2;
	class ALaserGenerator* LaserGenerator1_3;
	class ALaserGenerator* LaserGenerator1_4;
	class ALaserGenerator* LaserGenerator2_1;
	class ALaserGenerator* LaserGenerator2_2;
	class ALaserGenerator* LaserGenerator2_3;
	class ALaserGenerator* LaserGenerator2_4;




//
// Setup Function
//
private:
	void SetupGame();

	void SetupTimer();
	FTimerHandle TimerHandle;

	UFUNCTION()
	void SpawnLaser(int nPlayer, int nGenerator);

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

//
// UI
//
	virtual void ReturnToMainMenuHost() override;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevel;

};
