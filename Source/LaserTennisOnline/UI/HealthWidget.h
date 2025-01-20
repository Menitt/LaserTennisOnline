// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Text;

	FString TextToDisplay;

public:
	void UpdateHealthDisplayed(int Health);

	void InitializeText(const FString& InitialText, int PlayerHealth);

};
