// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayer.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"



void ALocalMultiplayer::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    TArray<AActor*> TempArray;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassPlayer1, TempArray);
    if (TempArray.Num() > 0)
    {
        Player1 = Cast<ABasePlayer>(TempArray[0]);
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassPlayer2, TempArray);
    if (TempArray.Num() > 0)
    {
        Player2 = Cast<ABasePlayer>(TempArray[0]);
    }

    if (NewPlayer)
    {
        NewPlayer->Possess(Player1);
    }
    

    APlayerController* NewPlayerController = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
    
    if (NewPlayerController)
    {
        NewPlayerController->Possess(Player2);
    }
    


}