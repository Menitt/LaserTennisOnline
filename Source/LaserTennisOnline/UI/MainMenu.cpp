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
    
    if (SinglePlayerButton)
    {
        SinglePlayerButton->SetIsEnabled(false);
    }
    
    FTimerHandle OpenLevelTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(OpenLevelTimerHandle, this, &ThisClass::OpenSinglePlayerLevel, .2f, false);

    // FString OptionsString = FString::Printf(TEXT("?game=%s"), *SinglePlayerGameMode);
    
    // UGameplayStatics::OpenLevel(this, GameLevelName, true, OptionsString);

    // MenuTearDown();

}

void UMainMenu::OpenSinglePlayerLevel()
{
    MenuTearDown();
    
    FString OptionsString = FString::Printf(TEXT("?game=%s"), *SinglePlayerGameMode);
    
    UGameplayStatics::OpenLevel(this, GameLevelName, true, OptionsString);    
}

void UMainMenu::LocalMultiplayerButtonClicked()
{
    PlayUISound();

    if (LocalMultiplayerButton)
    {
        LocalMultiplayerButton->SetIsEnabled(false);
    }
    
    FTimerHandle OpenLevelTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(OpenLevelTimerHandle, this, &ThisClass::OpenLocalMultiplayerLevel, .2f, false);

}

void UMainMenu::OpenLocalMultiplayerLevel()
{
    MenuTearDown();

    FString OptionsString = FString::Printf(TEXT("?game=%s"), *LocalMultiplayerGameMode);
    
    UGameplayStatics::OpenLevel(this, GameLevelName, true, OptionsString);
}

void UMainMenu::OnlineMultiplayerButtonClicked()
{
    PlayUISound();
    
    if (OnlineMultiplayerButton)
    {
        OnlineMultiplayerButton->SetIsEnabled(false);
    }

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
    if (QuitButton)
    {
        QuitButton->SetIsEnabled(false);
    }
    
	if (GEngine)
    {
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }
}


void UMainMenu::NativeDestruct()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Purple, TEXT("UMainMenu->NativeDestruct!"));
    }
    Super::NativeDestruct();

}