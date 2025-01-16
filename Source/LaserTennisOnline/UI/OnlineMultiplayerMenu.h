// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "OnlineMultiplayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UOnlineMultiplayerMenu : public UMenu
{
	GENERATED_BODY()
	

	virtual bool Initialize() override;

	virtual void NativeDestruct() override;

private:
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UFUNCTION()
	void BackButtonClicked();

};
