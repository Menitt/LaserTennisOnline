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
	class UButton* LocalMultiplayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OnlineMultiplayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

public:
	virtual void MenuSetup() override;

	virtual void NativeDestruct() override;

private:
	FString SinglePlayerGameMode = "/Game/GameModes/BP_SinglePlayer.BP_SinglePlayer_C";
	FString LocalMultiplayerGameMode = "/Game/GameModes/BP_LocalMultiplayer.BP_LocalMultiplayer_C";
	FString PathToLobby{TEXT("/Game/Maps/GameMapOnline")};

	UPROPERTY(EditDefaultsOnly)
	FName GameLevelName;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OnlineMultiplayerWidgetClass;

	UFUNCTION()
	void SinglePlayerButtonClicked();

	UFUNCTION()
	void LocalMultiplayerButtonClicked();

	UFUNCTION()
	void OnlineMultiplayerButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

};
