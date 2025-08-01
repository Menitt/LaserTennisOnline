// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasePlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "Math/UnrealMathUtility.h"
#include "GameStartPanel.h"
#include "HealthPanel.h"
#include "GameFramework/GameStateBase.h"
#include "CountDownWidget.h"
#include "MegaScreen.h"


void ALaserTennisGameModeBase::SetupGame()
{
    TArray<AActor*> TempArray;

    // Get Laser Platforms
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserActivationPlatformClass,"1",
    laserPlatforms1);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserActivationPlatformClass,"2",
    laserPlatforms2);

    ActiveLaserPlatforms1.Init(1,laserPlatforms1.Num());
    ActiveLaserPlatforms2.Init(1,laserPlatforms2.Num());

    // Get Health Panels
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),AHealthPanel::StaticClass(),"1",TempArray);
    if (TempArray.Num() > 0)
    {
        HealthPanel1 = Cast<AHealthPanel>(TempArray[0]);
    }
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),AHealthPanel::StaticClass(),"2",TempArray);
    if (TempArray.Num() > 0)
    {
        HealthPanel2 = Cast<AHealthPanel>(TempArray[0]);
    }

    // Get Mega Screen
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMegaScreen::StaticClass(),TempArray);
    if (TempArray.Num() > 0)
    {
        MegaScreen = Cast<AMegaScreen>(TempArray[0]);
    }



}

void ALaserTennisGameModeBase::SetupTimer()
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALaserTennisGameModeBase::ManagePlatforms, 0.5f, true);
}

void ALaserTennisGameModeBase::ManagePlatforms()
{
    
    UpdateActivePlatformsList(ActiveLaserPlatforms1, laserPlatforms1);
    UpdateActivePlatformsList(ActiveLaserPlatforms2, laserPlatforms2);

    AdjustPlatforms(ActiveLaserPlatforms1, laserPlatforms1);
    AdjustPlatforms(ActiveLaserPlatforms2, laserPlatforms2);
}

void ALaserTennisGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALaserTennisGameModeBase::UpdateActivePlatformsList(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList)
{
    int nPlatforms = PlatformsMap.Num();
    for (int i=0; i<nPlatforms; ++i)
    {
        AActor* Actor = PlatformList[i];
        ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
        if (Platform and Platform->IsPlatformActive())
        {
            PlatformsMap[i] = 1;
        }
        else if (Platform and Platform->IsPlatformActivating())
        {
            PlatformsMap[i] = 2;
        }
        else if (Platform and Platform->IsPlatformResting())
        {
            PlatformsMap[i] = -1;
        }
        else if (Platform and Platform->IsPlatformDeactivating())
        {
            PlatformsMap[i] = -2;
        }
    }

}

void ALaserTennisGameModeBase::AdjustPlatforms(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList)
{
    int nActive = GetNumberPlatformsByKey(PlatformsMap,1) + GetNumberPlatformsByKey(PlatformsMap,2);

    int nAdjust = nActivePlatforms - nActive;
    if (nAdjust > 0)
    {
        ActivatePlatform(PlatformsMap, PlatformList);
    }
    if (nAdjust < 0)
    {
        DeactivatePlatform(PlatformsMap, PlatformList);
    }
}

void ALaserTennisGameModeBase::ActivatePlatform(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList)
{
    int nRestingPlatforms = GetNumberPlatformsByKey(PlatformsMap,-1);

    int32 RandomInt = FMath::RandRange(1, nRestingPlatforms);

    int index = 0;
    for (int i=0; i<PlatformsMap.Num(); ++i)
    {
        if (PlatformsMap[i] == -1)
        {
            index++;
        }
        
        if (index == RandomInt)
        {
            AActor* Actor = PlatformList[i];
            ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
            if (Platform)
            {
                Platform->Activate(); 
                PlatformsMap[i] = 2;       
            }
            // PlatformsMap[i] = 2;

            return;
        }      
    }
}

void ALaserTennisGameModeBase::DeactivatePlatform(TArray<int>& PlatformsMap, TArray<AActor*>& PlatformList)
{
    
    int32 RandomInt = FMath::RandRange(1, GetNumberPlatformsByKey(PlatformsMap,1));

    int index = 0;
    for (int i=0; i<PlatformsMap.Num(); ++i)
    {
        if (PlatformsMap[i] == 1)
        {
            index++;
        }
        
        if (index == RandomInt)
        {
            AActor* Actor = PlatformList[i];
            ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
            if (Platform)
            {
                Platform->Deactivate();
                PlatformsMap[i] = -2;        
            }
            return;
        }        
    }
}

int ALaserTennisGameModeBase::GetNumberPlatformsByKey(const TArray<int>& PlatformsMap, int key) const
{
    int counter = 0;
    for (int k : PlatformsMap)
    {
        if (k == key)
        {
            counter++;
        }
    }

    return counter;
}

void ALaserTennisGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    
    Super::PostLogin(NewPlayer);
    PlayerCount++;
    if (PlayerCount == 1)
    {
        SetupGame();
    }

}

void ALaserTennisGameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void ALaserTennisGameModeBase::GameOver()
{
    EndMatch();
}

void ALaserTennisGameModeBase::HandleMatchHasEnded()
{
    Super::HandleMatchHasEnded();

    if (IsValid(Player1))
    {
        Player1->GameOver(Player1->bIsAlive());
    }
    if (IsValid(Player2))
    {
        Player2->GameOver(Player2->bIsAlive());
    }

}

void ALaserTennisGameModeBase::ReturnToMainMenuHost()
{

    // UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevel);

    Super::ReturnToMainMenuHost();
}

void ALaserTennisGameModeBase::StartGame()
{
    SetupTimer();

    UpdateHealthPanel();
}

void ALaserTennisGameModeBase::StartCountdown()
{
    

    // Setup Platforms
    for (int i=0; i<laserPlatforms1.Num(); ++i)
    {
        ManagePlatforms();
    }

    // Setup HealthPanels
    if (IsValid(HealthPanel1))
    {
        HealthPanel1->Activate(CountdownTime);
    }
    if (IsValid(HealthPanel2))
    {
        HealthPanel2->Activate(CountdownTime);
    }

    // Start Countfown on Screen
    if (IsValid(MegaScreen))
    {
        MegaScreen->StartCountdown(CountdownTime);
    }

}

void ALaserTennisGameModeBase::DelayedStartCountdown()
{
    // Only used in Online multiplayer
    // Set the timer to call MyFunction every 1 second, for 5 seconds
    FTimerHandle NewTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(NewTimerHandle, this, &ALaserTennisGameModeBase::StartCountdown, 0.5f, false);
}


void ALaserTennisGameModeBase::UpdateHealthPanel()
{
    // Update Health Widgets
    if (HealthPanel1 and HealthPanel1->IsValidLowLevel() and Player1 and Player1->IsValidLowLevel())
    {
       HealthPanel1->UpdateWidgetHealth(Player1->GetPlayerHealth()); 
    }
    if (HealthPanel2 and HealthPanel2->IsValidLowLevel() and Player2 and Player2->IsValidLowLevel())
    {
       HealthPanel2->UpdateWidgetHealth(Player2->GetPlayerHealth()); 
    }
}

void ALaserTennisGameModeBase::DisablePlayerInput(ABasePlayer* Player)
{
    // pass
}

void ALaserTennisGameModeBase::EnablePlayerInput(ABasePlayer* Player)
{
    // pass
}