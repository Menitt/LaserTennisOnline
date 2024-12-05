// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FString GameLevel;


	void InitiateStartGame();
	void StartGame();

	FTimerHandle TimerHandle;



};