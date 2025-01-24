// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineMultiplayer.h"
#include "Kismet/GameplayStatics.h"
#include "LocalMultiplayer.h"


void UInGameMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonClicked);
	}

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &ThisClass::ResumeButtonClicked);
    }

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		}
	}
}


bool UInGameMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	return true;
}


void UInGameMenu::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		MainMenuButton->SetIsEnabled(true);
		return;
	}
	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}


void UInGameMenu::MenuTearDown()
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


void UInGameMenu::MainMenuButtonClicked()
{
	MainMenuButton->SetIsEnabled(false);
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();
	AOnlineMultiplayer* OnlineMultGameMode = Cast<AOnlineMultiplayer>(GameMode);
	ALocalMultiplayer* LocalMultGameMode = Cast<ALocalMultiplayer>(GameMode);

	if (MultiplayerSessionsSubsystem and OnlineMultGameMode)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
	else if (LocalMultGameMode)
	{
		LocalMultGameMode->ReturnToMainMenuHost();
	}
	else if (GameMode)
	{
		GameMode->ReturnToMainMenuHost();
	}
}

void UInGameMenu::ResumeButtonClicked()
{
    MenuTearDown();
}

