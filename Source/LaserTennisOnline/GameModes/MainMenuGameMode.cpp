// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"


AMainMenuGameMode::AMainMenuGameMode()
{
    UE_LOG(LogTemp, Warning, TEXT("Main Menu Game Mode: Constructor!"));

    bUseSeamlessTravel = true;
}



void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
    if (nPlayers > 0)
    {
        NewPlayer->Destroy();
    }
    else
    {
        Super::PostLogin(NewPlayer);
        nPlayers++;
    }
    
}