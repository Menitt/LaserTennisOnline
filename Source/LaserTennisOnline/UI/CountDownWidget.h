// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "CountDownWidget.generated.h"

/**
 * 
 */

// Declare a multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountdownDelegate);

UCLASS()
class LASERTENNISONLINE_API UCountDownWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:

	FCountdownDelegate OnCountdownComplete;

	void StartCountdown(int Time);

	virtual void MenuSetup() override;

private:

	int Counter = -1;

	void UpdateCountdown();

	void StartGame();

	UPROPERTY(meta = (BindWidget)) class UMultiLineEditableTextBox* CountdownText;

	UPROPERTY(EditDefaultsOnly) bool IsPermanent = false;	
	UPROPERTY(EditDefaultsOnly) float FontSize = 250;

	FTimerHandle TimerHandle;

	void BroadcastGameStart();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Color")
	FVector TextColorRGB;

};
