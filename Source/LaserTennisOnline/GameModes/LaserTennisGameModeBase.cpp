// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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