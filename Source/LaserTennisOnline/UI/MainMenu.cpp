// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "OnlineMultiplayerMenu.h"

void UMainMenu::MenuSetup()
{
    Super::MenuSetup();

    if (SinglePlayerButton)
    {
        SinglePlayerButton->OnClicked.AddDynamic(this, &ThisClass::SinglePlayerButtonClicked);
    }

    if (LocalMultiplayerButton)
    {
        LocalMultiplayerButton->OnClicked.AddDynamic(this, &ThisClass::LocalMultiplayerButtonClicked);
    }

    if (OnlineMultiplayerButton)
    {
        OnlineMultiplayerButton->OnClicked.AddDynamic(this, &ThisClass::OnlineMultiplayerButtonClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
    }
}

void UMainMenu::SinglePlayerButtonClicked()
{
    PlayUISound();
    
    SinglePlayerButton->SetIsEnabled(false);

    FString OptionsString = FString::Printf(TEXT("?game=%s"), *SinglePlayerGameMode);
    
    UGameplayStatics::OpenLevel(this, GameLevelName, true, OptionsString);

    MenuTearDown();
}

void UMainMenu::LocalMultiplayerButtonClicked()
{
    PlayUISound();

    LocalMultiplayerButton->SetIsEnabled(false);

    FString OptionsString = FString::Printf(TEXT("?game=%s"), *LocalMultiplayerGameMode);
    
    UGameplayStatics::OpenLevel(this, GameLevelName, true, OptionsString);

    MenuTearDown();
}


void UMainMenu::OnlineMultiplayerButtonClicked()
{
    PlayUISound();
    OnlineMultiplayerButton->SetIsEnabled(false);
 
    UOnlineMultiplayerMenu* OnlineMultiplayerMenuWidget = CreateWidget<UOnlineMultiplayerMenu>(GetWorld(), OnlineMultiplayerWidgetClass);

    if (OnlineMultiplayerMenuWidget)
    {
        MenuTearDown();
        OnlineMultiplayerMenuWidget->MenuSetup(4,"",PathToLobby);
    }

    
}

void UMainMenu::QuitButtonClicked()
{

    PlayUISound();
    QuitButton->SetIsEnabled(false);

	if (GEngine)
    {
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }
}


void UMainMenu::NativeDestruct()
{
    Super::NativeDestruct();

    UE_LOG(LogTemp, Warning, TEXT("Main Menu Destructor!"));
}