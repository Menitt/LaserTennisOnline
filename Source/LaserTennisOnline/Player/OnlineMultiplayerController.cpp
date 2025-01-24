// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMultiplayerController.h"
#include "BasePlayer.h"

AOnlineMultiplayerController::AOnlineMultiplayerController()
{
    
    bReplicates = true;

}


void AOnlineMultiplayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        ABasePlayer* PlayerPawn = Cast<ABasePlayer>(GetPawn());

        if (Player)
        {
            PlayerPawn->EnableEnhancedInputSystem(this);
            UE_LOG(LogTemp, Warning, TEXT("Enabling Input from Online Multiplayer Controller"));
        }
    }
}

