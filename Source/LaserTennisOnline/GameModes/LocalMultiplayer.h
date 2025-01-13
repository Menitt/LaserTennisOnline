// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserTennisGameModeBase.h"
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

//
// INPUT
//
	// Input Map Context
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* LocalMultiplayer_InputMapContext;

	void HandleInputAssignment();

	class APlayerController* PlayerController1;
	class APlayerController* PlayerController2;

	// Players Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P1_MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P1_JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P1_DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P2_MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P2_JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* P2_DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Players", meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseGameAction;



};
