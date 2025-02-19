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

	// Bind Sound File
	FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	Sound = LoadObject<USoundWave>(nullptr, *SoundPath);

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

	if (PlayerController)
    {	
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn and Sound)
		{
			FVector SoundLocation = PlayerPawn->GetActorLocation(); // You can set a custom location
			
			UGameplayStatics::PlaySoundAtLocation(this, Sound, SoundLocation,ScaleVolume,ScalePitch,StartTime);
		}
    }
}