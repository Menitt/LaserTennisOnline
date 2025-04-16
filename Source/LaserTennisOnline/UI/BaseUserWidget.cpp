// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "UObject/UObjectGlobals.h"


void UBaseUserWidget::MenuSetup()
{
    AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		
		}
		
	}
}

bool UBaseUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	// Load Sound File
	FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	Sound = LoadObject<USoundWave>(nullptr, *SoundPath);

	if (Sound and Sound->IsValidLowLevel())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("UBaseUserWidget->Initialize: Sound is loaded and valid!"));
		}
		
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("UBaseUserWidget->Initialize: Sound File is not valid!"));
		}
	}


	return true;
}

void UBaseUserWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UBaseUserWidget::PlayUISound()
{
	
	UWorld* World = GetWorld();
	bool ValidSound = World and Sound and Sound->IsValidLowLevel();

	if (ValidSound)
	{		
		UGameplayStatics::PlaySound2D(World, Sound, ScaleVolume, ScalePitch, StartTime);
	}
	else if (World and Sound and !Sound->IsValidLowLevel())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("UBaseUserWidget->PlayUISound: Could not play Sound due to bad Sound File!"));
		}
	}
	
}