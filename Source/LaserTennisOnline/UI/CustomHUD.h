// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ACustomHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

protected:
	int PlayerHealth = 5;
	int PlayerMaxHealth = 5;

public:

	UPROPERTY(EditDefaultsOnly)
	float ScreenLeftOffset = 100.;
	UPROPERTY(EditDefaultsOnly)
	float ScreenBotOffset = 100.;
	UPROPERTY(EditDefaultsOnly)
	FVector2D FontSize = FVector2D(1.,1.);

public:
	void UpdatePlayerHealth(int NewHealth);
	void UpdatePlayerMaxHealth(int MaxHealth){PlayerMaxHealth = MaxHealth;};

	UFUNCTION(BlueprintCallable)
	int GetPlayerHealth(){return PlayerHealth;};


};
