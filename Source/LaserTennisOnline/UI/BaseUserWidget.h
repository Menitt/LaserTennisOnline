// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

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
	UPROPERTY(EditDefaultsOnly)
	FString ButtonSound;

	void PlayButtonSound();


};
