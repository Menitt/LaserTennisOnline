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

	virtual void HostButtonClicked() override;

	virtual void JoinButtonClicked() override;

	virtual void FriendsButtonClicked() override;


// Sound
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	FString SoundFile;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float StartTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ScaleVolume = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ScalePitch = 1.f;

	FString SoundFolder = "/Game/Assets/Audio/UI/";
	class USoundWave* Sound;
	void PlayUISound();

};
