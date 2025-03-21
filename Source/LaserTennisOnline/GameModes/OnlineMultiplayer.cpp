// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMultiplayer.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayer.h"
#include "GameFramework/PlayerStart.h"
#include "OnlineMultiplayerController.h"
#include "GameFramework/GameStateBase.h"
#include "LaserActivationPlatform.h"

void AOnlineMultiplayer::PostLogin(APlayerController* NewPlayer)
{

    Super::PostLogin(NewPlayer);

    // Destroy Default Player Pawn
    APawn* Pawn = NewPlayer->GetPawn();
    if (Pawn)
    {
        NewPlayer->UnPossess();
        Pawn->Destroy();
    }
    
    TArray<AActor*> TempArray;
    // Assing Player1 to Server    
    if (NewPlayer->IsLocalController())
    {
        PlayerController1 = NewPlayer;
        // Find Player 1 Start
        UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "1", TempArray);
        AActor* PlayerStart1 = nullptr;
        if (TempArray.Num() > 0)
        {
            PlayerStart1 = TempArray[0];
        }

        // Spawn and Possess Player1
        if (PlayerStart1)
        {
            Player1 = GetWorld()->SpawnActor<ABasePlayer>(ClassPlayer1,PlayerStart1->GetActorLocation(), 
            PlayerStart1->GetActorRotation());
            NewPlayer->Possess(Player1);
        }
    }
    // Assign Player 2 to Client
    else
    {
        PlayerController2 = NewPlayer;
        // Find Player 2 Start
        UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "2", TempArray);
        AActor* PlayerStart2 = nullptr;
        if (TempArray.Num() > 0)
        {
            PlayerStart2 = TempArray[0];
        }

        // Spawn and Possess Player1
        if (PlayerStart2)
        {
            Player2 = GetWorld()->SpawnActor<ABasePlayer>(ClassPlayer2,PlayerStart2->GetActorLocation(), 
            PlayerStart2->GetActorRotation());
            NewPlayer->Possess(Player2);
        }
    }  

    if (PlayerCount==2)
    {
        DelayedStartCountdown();
    }

}

void AOnlineMultiplayer::BeginPlay()
{
    Super::BeginPlay();

    SetupTimer();
}


void AOnlineMultiplayer::StartCountdown()
{
    UE_LOG(LogTemp, Warning, TEXT("Online Multiplayer: Start Countdown!"));
    for (int i=0; i<laserPlatforms1.Num(); ++i)
    {
        ALaserActivationPlatform* Platform1 = Cast<ALaserActivationPlatform>(laserPlatforms1[i]);
        ALaserActivationPlatform* Platform2 = Cast<ALaserActivationPlatform>(laserPlatforms2[i]);
        if (Platform1 and Platform2)
        {
            Platform1->Activate();
            Platform2->Activate();
        }
    }
    
    Super::StartCountdown();

    if (Player1)
    {
        Player1->StartCountdown(CountdownTime);
    }
    if (Player2)
    {
        Player2->StartCountdown(CountdownTime);
    }

}

void AOnlineMultiplayer::StartGame()
{
    Super::StartGame();

    if (Player1)
    {
        Player1->StartGame();
    }
    if (Player2)
    {
        Player2->StartGame();
    }

}