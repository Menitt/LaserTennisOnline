// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"



void UGameOverWidget::MenuSetup()
{
	Super::MenuSetup();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		}
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonClicked);
	}

}


void UGameOverWidget::OnDestroySession(bool bWasSuccessful)
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


void UGameOverWidget::MainMenuButtonClicked()
{
	MainMenuButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}