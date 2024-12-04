// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasePlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "Math/UnrealMathUtility.h"

void ALaserTennisGameModeBase::SetupGame()
{
    TArray<AActor*> TempArray;
    
    // Get Players
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),ClassPlayer1,TempArray);
    if (TempArray.Num() > 0)
    {
        Player1 = Cast<ABasePlayer>(TempArray[0]);
    }
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),ClassPlayer2,TempArray);
    if (TempArray.Num() > 0)
    {
        Player2 = Cast<ABasePlayer>(TempArray[0]);
    }
    
    
    // Get All Gameplay Actors
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserGeneratorClass,"1",
    laserGenerators1);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserGeneratorClass,"2",
    laserGenerators2);

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserActivationPlatformClass,"1",
    laserPlatforms1);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserActivationPlatformClass,"2",
    laserPlatforms2);


    ActiveLaserPlatforms1.Init(1,laserPlatforms1.Num());
    ActiveLaserPlatforms2.Init(1,laserPlatforms2.Num());


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

    SetupTimer();

}


void ALaserTennisGameModeBase::SpawnLaserRequest(FName PlayerTag)
{

    // Pick one generator randomly:
    int nGenerators1 = laserGenerators1.Num();
    int nGenerators2 = laserGenerators2.Num();
    ALaserGenerator* Generator;

    int RandInt;
    int PlayerID;

    if (PlayerTag == "1")
    {
        RandInt = UKismetMathLibrary::RandomInteger(nGenerators2);
        Generator = Cast<ALaserGenerator>(laserGenerators2[RandInt]);
        PlayerID = 1;
    }
    else 
    {
        RandInt = UKismetMathLibrary::RandomInteger(nGenerators1);
        Generator = Cast<ALaserGenerator>(laserGenerators1[RandInt]);
        PlayerID = 2;
    }

    // if (Generator)
    // {
    //     Generator->SpawnLaser();
    // }

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
            BasePlayer->GameOver(BasePlayer->bIsAlive());
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

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),laserGeneratorClass,LookupTag,TempArray);
    if (TempArray.Num() > 0)
    {
        ALaserGenerator* LaserGenerator = Cast<ALaserGenerator>(TempArray[0]);
        if (LaserGenerator)
        {
            LaserGenerator->SpawnLaser();
        }
    }
}