// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"

#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UGameOverWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void MenuSetup() override;

protected:

	// Main Menu Button
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UButton> MainMenuButton;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevel;


	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private:

	UFUNCTION()
	void MainMenuButtonClicked();

private:
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;


};
