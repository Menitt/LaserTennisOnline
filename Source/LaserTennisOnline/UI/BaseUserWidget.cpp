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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("UBaseUserWidget->MenuSetup"));
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

	if (Sound and GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("UBaseUserWidget->Initialize, Sound is Loaded"));
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

	if (World)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("UBaseUserWidget->PlayUISound, World is valid"));
		}
		
		APlayerController* CurrentPlayerController = World->GetFirstPlayerController();
    
		if (CurrentPlayerController)
		{	
			APawn* PlayerPawn = CurrentPlayerController->GetPawn();
			if (PlayerPawn and UISound)
			{
				
				if (!Sound->IsValidLowLevel() || Sound->IsPendingKill())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("UBaseUserWidget->PlayUISound: Sound is invalid or pending kill!"));
					return;
				}
				
				FVector SoundLocation = PlayerPawn->GetActorLocation(); // You can set a custom location
				
				// UGameplayStatics::PlaySoundAtLocation(World, Sound, SoundLocation,ScaleVolume,ScalePitch,StartTime);
				UGameplayStatics::PlaySound2D(PlayerPawn->GetWorld(), UISound, ScaleVolume, ScalePitch, StartTime);
			}		
		}

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("UBaseUserWidget->PlayUISound, World is not valid"));
		}
	}
	
}