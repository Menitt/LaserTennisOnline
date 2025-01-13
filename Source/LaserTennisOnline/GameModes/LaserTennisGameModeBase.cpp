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

    // Get Central Generator
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),CentralGeneratorClass,TempArray);
    if (TempArray.Num() > 0)
    {
        CentralGenerator = Cast<ACentralGenerator>(TempArray[0]);
        CentralGenerator->OnSignalArrived.AddDynamic(this, &ThisClass::SpawnLaser);
    }


    // Get Camera Actor
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),APawn::StaticClass(),"Default",TempArray);
    if (TempArray.Num() > 0)
    {
        DefaultPawn = Cast<APawn>(TempArray[0]);
    }


    // Get Game Start Panel
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),GameStartPanelClass,TempArray);
    if (TempArray.Num() > 0)
    {
        GameStartPanel = Cast<AGameStartPanel>(TempArray[0]);
        if (GameStartPanel)
        {
            GameStartPanel->OnGameStarting.AddDynamic(this, &ThisClass::StartGame);
        }
    }

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




}

void ALaserTennisGameModeBase::SetupTimer()
{
    // Set the timer to call MyFunction every 1 second, for 5 seconds
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALaserTennisGameModeBase::ManagePlatforms, 0.5f, true, 5.f);
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
        PlatformsMap[i] = 0;
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
    
    int nAdjust = nActivePlatforms - GetNumberPlatformsByKey(PlatformsMap,1) - GetNumberPlatformsByKey(PlatformsMap,2);
    

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
            }
            // PlatformsMap[i] = -2;

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

    // if (HasAuthority())
    // {
        
    //     APawn* Pawn = NewPlayer->GetPawn();
        
    //     if (Pawn)
    //     {
    //         NewPlayer->UnPossess();
    //         Pawn->Destroy();
    //     }

    //     // Get the PlayerStart actors in the level
    //     TArray<AActor*> PlayerStarts;
    //     if (bIsPlayer1)
    //     {
    //         UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "1", PlayerStarts);
    //     }
    //     else
    //     {
    //         UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "2", PlayerStarts);
    //     }
        
    //     // Randomly select a PlayerStart
    //     if (PlayerStarts.Num() > 0)
    //     {
            
    //         APlayerStart* SelectedStart = Cast<APlayerStart>(PlayerStarts[0]);
            
    //         if (SelectedStart)
    //         {
    //             // Spawn the player pawn at the selected start location
    //             FVector SpawnLocation = SelectedStart->GetActorLocation();
    //             FRotator SpawnRotation = SelectedStart->GetActorRotation();

    //             if (bIsPlayer1)
    //             {
    //                 // Assuming you have a reference to your player pawn class
    //                 ABasePlayer* NewPawn = GetWorld()->SpawnActor<ABasePlayer>(ClassPlayer1, SpawnLocation, SpawnRotation);
    //                 NewPlayer->Possess(NewPawn);
    //             }
    //             else
    //             {
    //                 // Assuming you have a reference to your player pawn class
    //                 ABasePlayer* NewPawn = GetWorld()->SpawnActor<ABasePlayer>(ClassPlayer2, SpawnLocation, SpawnRotation);
    //                 NewPlayer->Possess(NewPawn);
    //             }
    //         }
    //     }

    //     bIsPlayer1 = false;
    // }

    // int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	
    // if (NumberOfPlayers == 2)
	// {
    //     FTimerHandle TimerHandleGameStart;
    // 	// Set the timer to call MyFunction every 1 second, for 5 seconds
    //     GetWorld()->GetTimerManager().SetTimer(TimerHandleGameStart, this, &ALaserTennisGameModeBase::InitiateGameStart,0.5,false);
	// }
}


void ALaserTennisGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    SetupGame();

    SetupTimer();

}


void ALaserTennisGameModeBase::SpawnLaserRequest(int PlayerID)
{

    int RandInt = UKismetMathLibrary::RandomInteger(4) + 1;

    if (CentralGenerator)
    {
        CentralGenerator->SendSignal(PlayerID, RandInt);
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
            BasePlayer->GameOver(BasePlayer->bIsAlive(), DefaultPawn);
        }
    }

}

void ALaserTennisGameModeBase::ReturnToMainMenuHost()
{

    // UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevel);

    Super::ReturnToMainMenuHost();
}

void ALaserTennisGameModeBase::SpawnLaser(int nPlayer, int nGenerator)
{
    
    FString String1 = FString::FromInt(nPlayer);
    FString String2 = FString::FromInt(nGenerator);

    FName LookupTag(*(String1 + String2));
    
    TArray<AActor*> TempArray;

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),LaserGeneratorClass,LookupTag,TempArray);
    if (TempArray.Num() > 0)
    {
        ALaserGenerator* LaserGenerator = Cast<ALaserGenerator>(TempArray[0]);
        if (LaserGenerator)
        {
            LaserGenerator->SpawnLaser();
        }
    }
}

void ALaserTennisGameModeBase::StartGame()
{
    if (Player1)
    {
        Player1->GameStart();
    }
    if (Player2)
    {
        Player2->GameStart();
    }
}

void ALaserTennisGameModeBase::InitiateGameStart()
{
    // Get Players
    TArray<AActor*> TempArray;
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),ClassPlayer1,TempArray);
    if (TempArray.Num()>0)
    {
        Player1 = Cast<ABasePlayer>(TempArray[0]);
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(),ClassPlayer2,TempArray);
    if (TempArray.Num()>0)
    {
        Player2 = Cast<ABasePlayer>(TempArray[0]);
    }

    if (GameStartPanel)
    {
        GameStartPanel->StartCountdown();
    }
    if (Player1 and HealthPanel1)
    {
        Player1->GamePreStart();
        Player1->OnCustomTakeDamage.AddDynamic(HealthPanel1, &AHealthPanel::UpdateWidgetHealth);
        HealthPanel1->Activate();
        if (Player1->OnCustomTakeDamage.IsBound())
        {
            Player1->OnCustomTakeDamage.Broadcast(Player1->GetPlayerHealth());
        }
    }
    if (Player2 and HealthPanel2)
    {
        Player2->GamePreStart();
        Player2->OnCustomTakeDamage.AddDynamic(HealthPanel2, &AHealthPanel::UpdateWidgetHealth);
        HealthPanel2->Activate();

        if (Player2->OnCustomTakeDamage.IsBound())
        {
            Player2->OnCustomTakeDamage.Broadcast(Player2->GetPlayerHealth());
        }

    }
}


