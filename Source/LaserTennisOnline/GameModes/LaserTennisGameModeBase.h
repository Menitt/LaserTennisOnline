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


protected:
virtual void BeginPlay() override;

virtual void PostLogin(APlayerController* NewPlayer) override;

virtual void Tick(float DeltaTime) override;

//
// Gameplay Actors Containers
//
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AActor> LaserGeneratorClass;

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
	TSubclassOf<AActor> ClassPlayer1;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ClassPlayer2;
	bool bIsPlayer1 = true;
	class ABasePlayer* Player1;
	class ABasePlayer* Player2;


	class APawn* DefaultPawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> GameStartPanelClass;
	class AGameStartPanel* GameStartPanel;

	class AHealthPanel* HealthPanel1;
	class AHealthPanel* HealthPanel2;

	int PlayerCount = 0;

//
// Setup Function
//
private:
	void SetupGame();

	void SetupTimer();
	FTimerHandle TimerHandle;

	UFUNCTION()
	void SpawnLaser(int nPlayer, int nGenerator);

protected:
	virtual void StartCountdown();
//
// Gameplay
//
public:

	UFUNCTION()
	virtual void StartGame();

	void ManagePlatforms();

	void UpdateActivePlatformsList(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	void AdjustPlatforms(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	void ActivatePlatform(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	void DeactivatePlatform(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList);

	int GetNumberPlatformsByKey(const TArray<int>& PlatformsMap, int key) const;

	void SpawnLaserRequest(int PlayerID);

	void GameOver();

	virtual void HandleMatchHasEnded() override;

	void DelayedStartCountdown();

	UPROPERTY(EditDefaultsOnly)
	int nActivePlatforms;

//
// UI
//
	virtual void ReturnToMainMenuHost() override;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevel;

protected:
	UPROPERTY(EditDefaultsOnly)
	int CountdownTime = 3;


};
