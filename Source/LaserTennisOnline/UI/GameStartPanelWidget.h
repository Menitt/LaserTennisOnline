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
	class UTextBox* GameStartCountDown;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bStartCountdown = false;

public:
	void StartCountdown();

private:
	UFUNCTION(BlueprintCallable)
	void StartGame();

};
