// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserTennisGameModeBase.h"
#include "Blueprint/UserWidget.h"

#include "LocalMultiplayer.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ALocalMultiplayer : public ALaserTennisGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	virtual void PostLogin(class APlayerController* NewPlayer) override;

//
// INPUT
//
	// Input Map Context
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* LocalMultiplayer_InputMapContext;

	void HandleInputAssignment();

	// Players Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P1_MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P1_JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P1_DodgeAction;
	uint32 P1_MoveHandle;
	uint32 P1_JumpHandle;
	uint32 P1_DodgeHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P2_MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P2_JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P2_DodgeAction;
	uint32 P2_MoveHandle;
	uint32 P2_JumpHandle;
	uint32 P2_DodgeHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseGameAction;

	class UEnhancedInputComponent* SharedInputComponent;

	class APlayerController* SharedPlayerController;
	class APlayerController* PlayerController1;
	class APlayerController* PlayerController2;
	class ABasePlayer* SharedInputPawn;

protected:

	virtual void StartCountdown() override;

	virtual void HandleMatchHasEnded() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> Player1WinnerWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> Player2WinnerWidgetClass;

public:
	void RemovePlayers();

	virtual void DisablePlayerInput(class ABasePlayer* Player) override;
	virtual void EnablePlayerInput(class ABasePlayer* Player) override;

	virtual void ReturnToMainMenuHost() override;
	virtual void StartGame() override;

//
// TESTING
//
private:
	UPROPERTY(EditAnywhere, Category = "Testing") bool TestingFlag = false;



};
