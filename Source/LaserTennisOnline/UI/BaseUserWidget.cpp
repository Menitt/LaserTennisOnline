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


void UBaseUserWidget::PlayButtonSound()
{

	if (PlayerController)
    {	
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			USoundWave* Sound = LoadObject<USoundWave>(nullptr, *ButtonSound);

			if (Sound)
			{
				FVector SoundLocation = PlayerPawn->GetActorLocation(); // You can set a custom location
				UGameplayStatics::PlaySoundAtLocation(this, Sound, SoundLocation,1,1,0.13);
			}
		}
		
    }
}