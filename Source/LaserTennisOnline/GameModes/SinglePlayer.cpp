// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "AIPlayerController.h"
#include "BasePlayer.h"

void ASinglePlayer::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Store Player Controller
    PlayerController = NewPlayer;

    TArray<AActor*> TempArray;

    // Find Player Start Locations
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "1", TempArray);
    APlayerStart* PlayerStart1 = nullptr;
    if (TempArray.Num() > 0)
    {
        PlayerStart1 = Cast<APlayerStart>(TempArray[0]);
    }

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "2", TempArray);
    APlayerStart* PlayerStart2 = nullptr;
    if (TempArray.Num() > 0)
    {
        PlayerStart2 = Cast<APlayerStart>(TempArray[0]);
    }

    // Unpossess & Destroy default player pawn
    APawn* PlayerPawn = PlayerController->GetPawn();

    if (GetWorld() and PlayerPawn)
    {
        PlayerController->UnPossess();
        PlayerPawn->Destroy();
    }

    // Spawn Player Character & AI pawn and controller
    UWorld* World = GetWorld();
    if (World and PlayerStart1)
    {
        Player1 = World->SpawnActor<ABasePlayer>(ClassPlayer1,PlayerStart1->GetActorLocation(), 
        PlayerStart1->GetActorRotation());
    }

    if (World and PlayerStart2)
    {
        Player2 = World->SpawnActor<ABasePlayer>(ClassPlayer2,PlayerStart2->GetActorLocation(), 
        PlayerStart2->GetActorRotation());

        AIPlayerController = World->SpawnActor<AAIPlayerController>(AIControllerClass, PlayerStart2->GetActorLocation(), 
        PlayerStart2->GetActorRotation());
    }

    // Possess Pawns
    if (Player1 and PlayerController)
    {
        PlayerController->Possess(Player1);
    }

    if (Player2 and AIPlayerController)
    {
        AIPlayerController->Possess(Player2);
    }

    // Delay Initiate Countdown
    // StartCountdown();
    FTimerHandle StartGameTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &ThisClass::StartCountdown, .5f, false);

}


void ASinglePlayer::StartCountdown()
{
    Super::StartCountdown();

    if (Player1)
    {
        Player1->StartCountdown(CountdownTime);
    }

}

void ASinglePlayer::StartGame()
{
    Super::StartGame();

    if (Player1)
    {
        Player1->StartGame();
    }

    if (AIPlayerController)
    {
        AIPlayerController->StartGame();
    }
}


 