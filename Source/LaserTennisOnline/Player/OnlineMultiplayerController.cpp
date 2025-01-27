// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMultiplayerController.h"
#include "BasePlayer.h"

AOnlineMultiplayerController::AOnlineMultiplayerController()
{
    
    bReplicates = true;

}


AOnlineMultiplayerController::~AOnlineMultiplayerController()
{

    UE_LOG(LogTemp, Warning, TEXT("Online Player Controller Destructor!"));
}

void AOnlineMultiplayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        SetShowMouseCursor(false);
    }
}