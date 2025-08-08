// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"

#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void PostLogin(class APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

public:
	AMainMenuGameMode();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

};
