// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenu.h"

AMainMenuGameMode::AMainMenuGameMode()
{
    bUseSeamlessTravel = true;
}


void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Create Main Menu Widget
    UMainMenu* MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuWidgetClass);

    if (MainMenuWidget)
    {
        MainMenuWidget->MenuSetup();
    }

}