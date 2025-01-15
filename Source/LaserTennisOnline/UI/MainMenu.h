// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UMainMenu : public UBaseUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MultiplayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	virtual void MenuSetup() override;

	FString SinglePlayerGameMode = "Game/Modes/BP_SinglePlayer";
	UPROPERTY(EditDefaultsOnly)
	FString SinglePlayerLevelName;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseUserWidget> MultiplayerWidgetClass;

	UFUNCTION()
	void SinglePlayerButtonClicked();

	UFUNCTION()
	void MultiplayerButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

};
