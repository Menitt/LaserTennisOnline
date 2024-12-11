// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UGameStartPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Instructions;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* GameStartCountdown;

	FTimerHandle TimerHandle;

	int Counter = -1;
	int Countdown = 5;

	void UpdateCountdown();

protected:
	virtual void NativeConstruct() override;


public:
	void StartCountdown();

	class AGameStartPanel* PanelOwner;

private:
	UFUNCTION(BlueprintCallable)
	void StartGame();



};
