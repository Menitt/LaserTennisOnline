// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

	// Game Over Text
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> GameOverText;

	// Main Menu Button
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UButton> MainMenuButton;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevel;

public:

	// Set Menu Text
	void SetText(bool bGameWon);


	UFUNCTION()
	void MainMenuButtonClicked();


};
