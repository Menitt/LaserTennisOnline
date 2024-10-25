// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	void MenuSetup();
	void MenuTearDown();


protected:

	virtual bool Initialize() override;


	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);


private:

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* QuitButton;

	UFUNCTION()
	void ResumeButtonClicked();

	UFUNCTION()
	void MainMenuButtonClicked();




	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	UPROPERTY()
	class APlayerController* PlayerController;


};
