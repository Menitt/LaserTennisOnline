// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"



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