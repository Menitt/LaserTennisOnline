// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMainMenu::MenuSetup()
{
    Super::MenuSetup();

    if (SinglePlayerButton)
    {
        SinglePlayerButton->OnClicked.AddDynamic(this, &ThisClass::SinglePlayerButtonClicked);
    }

    if (MultiplayerButton)
    {
        MultiplayerButton->OnClicked.AddDynamic(this, &ThisClass::MultiplayerButtonClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
    }
}

void UMainMenu::SinglePlayerButtonClicked()
{
    SinglePlayerButton->SetIsEnabled(false);

    FString OptionsString = FString::Printf(TEXT("?game=%s"), *SinglePlayerGameMode);
    
    UGameplayStatics::OpenLevel(this, *SinglePlayerLevelName, true, OptionsString);

    MenuTearDown();
}

void UMainMenu::MultiplayerButtonClicked()
{
    MultiplayerButton->SetIsEnabled(false);
    
    UBaseUserWidget* MultiplayerMenuWidget = CreateWidget<UBaseUserWidget>(GetWorld(), MultiplayerWidgetClass);

    MultiplayerMenuWidget->MenuSetup();

    MenuTearDown();
}

void UMainMenu::QuitButtonClicked()
{
    QuitButton->SetIsEnabled(false);

	if (GEngine)
    {
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }
}