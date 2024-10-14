// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasePlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"

void ALaserTennisGameModeBase::SetupGame()
{
    // Get All Gameplay Actors
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserGeneratorClass,"1",
    laserGenerators1);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserGeneratorClass,"2",
    laserGenerators2);

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserActivationPlatformClass,"1",
    laserPlatforms1);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserActivationPlatformClass,"2",
    laserPlatforms2);
    
    for (auto actor : laserGenerators1)
    {
        // DrawDebugSphere(GetWorld(), actor->GetActorLocation(), 100, 20, FColor::Red,
        // true);
    }
    for (auto actor : laserPlatforms1)
    {
        // DrawDebugSphere(GetWorld(), actor->GetActorLocation(), 100, 20, FColor::Red,
        // true);
    }
    
    return;
}


void ALaserTennisGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("ALaserTennisGameModeBase->PostLogin()"));

    if (HasAuthority())
    {
        
        APawn* Pawn = NewPlayer->GetPawn();
        
        if (Pawn)
        {
            NewPlayer->UnPossess();
            Pawn->Destroy();
            UE_LOG(LogTemp, Warning, TEXT("Destroy Pawn"));
        }

        // Get the PlayerStart actors in the level
        TArray<AActor*> PlayerStarts;
        if (bIsPlayer1)
        {
            UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "1", PlayerStarts);
        }
        else
        {
            UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "2", PlayerStarts);
        }
        
        // Randomly select a PlayerStart
        if (PlayerStarts.Num() > 0)
        {
            
            APlayerStart* SelectedStart = Cast<APlayerStart>(PlayerStarts[0]);
            
            if (SelectedStart)
            {
                // Spawn the player pawn at the selected start location
                FVector SpawnLocation = SelectedStart->GetActorLocation();
                FRotator SpawnRotation = SelectedStart->GetActorRotation();

                if (bIsPlayer1)
                {
                    // Assuming you have a reference to your player pawn class
                    ABasePlayer* NewPawn = GetWorld()->SpawnActor<ABasePlayer>(ClassPlayer1, SpawnLocation, SpawnRotation);
                    NewPlayer->Possess(NewPawn);
                }
                else
                {
                    // Assuming you have a reference to your player pawn class
                    ABasePlayer* NewPawn = GetWorld()->SpawnActor<ABasePlayer>(ClassPlayer2, SpawnLocation, SpawnRotation);
                    NewPlayer->Possess(NewPawn);
                }
            }
        }

        bIsPlayer1 = false;
    }

    
}






void ALaserTennisGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    SetupGame();
}


void ALaserTennisGameModeBase::SpawnLaserRequest(FName PlayerTag)
{
    UE_LOG(LogTemp, Display, TEXT("Inside GameMode->SpawnLaserRequest"));

    // Pick one generator randomly:
    int nGenerators1 = laserGenerators1.Num();
    int nGenerators2 = laserGenerators2.Num();
    ALaserGenerator* Generator;

    if (PlayerTag == "1")
    {
        int RandInt = UKismetMathLibrary::RandomInteger(nGenerators2);
        Generator = Cast<ALaserGenerator>(laserGenerators2[RandInt]);
    }
    else 
    {
        int RandInt = UKismetMathLibrary::RandomInteger(nGenerators1);
        Generator = Cast<ALaserGenerator>(laserGenerators1[RandInt]);
    }

    if (Generator)
    {
        Generator->SpawnLaser();
    }
}


void ALaserTennisGameModeBase::GameOver()
{
    EndMatch();
}

void ALaserTennisGameModeBase::HandleMatchHasEnded()
{
    Super::HandleMatchHasEnded();

    GEngine->AddOnScreenDebugMessage(-1,3.,FColor::Red,"HandleMatchHasEnded!");	

    TArray<AActor*> Players;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayer::StaticClass(), Players);

    GEngine->AddOnScreenDebugMessage(-1,3.,FColor::Red,"ALaserTennisGameModeBase->GameOver");

    for (AActor* player : Players)
    {
        ABasePlayer* BasePlayer = Cast<ABasePlayer>(player);
        if (BasePlayer)
        {
            BasePlayer->GameOver(BasePlayer->bIsAlive());
        }
    }

}