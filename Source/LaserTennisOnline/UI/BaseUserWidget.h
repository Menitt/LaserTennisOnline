// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundWave.h"

#include "BaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	virtual void MenuSetup();
	virtual void MenuTearDown();


protected:

	class APlayerController* PlayerController;
	virtual bool Initialize() override;


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
	UPROPERTY()	class USoundWave* Sound;

	void PlayUISound();


};
