// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasePlayer.h"

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
        DrawDebugSphere(GetWorld(), actor->GetActorLocation(), 100, 20, FColor::Red,
        true);
    }
    for (auto actor : laserPlatforms1)
    {
        DrawDebugSphere(GetWorld(), actor->GetActorLocation(), 100, 20, FColor::Red,
        true);
    }
    
    return;
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
    TArray<AActor*> Players;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayer::StaticClass(), Players);

    for (AActor* player : Players)
    {
        ABasePlayer* BasePlayer = Cast<ABasePlayer>(player);
        if (BasePlayer)
        {
            BasePlayer->GameOver(true);
        }
    }
}