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

    return;
}


void ALaserTennisGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateActivePlatformsList();

    AdjustPlatforms();
}



void ALaserTennisGameModeBase::UpdateActivePlatformsList()
{
    int nPlatforms1 = laserPlatforms1.Num();
    int nPlatforms2 = laserPlatforms2.Num();
    
    for (int i=0; i<nPlatforms1; ++i)
    {
        AActor* Actor = laserPlatforms1[i];
        ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
        if (Platform)
        {
            ActiveLaserPlatforms1[i] = (Platform->IsPlatformActive()) ? 1 : 0;
        }
    }

    for (int i=0; i<nPlatforms2; ++i)
    {
        AActor* Actor = laserPlatforms2[i];
        ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
        if (Platform)
        {
            ActiveLaserPlatforms2[i] = (Platform->IsPlatformActive()) ? 1 : 0;
        }
    }
}



void ALaserTennisGameModeBase::AdjustPlatforms()
{
    
    //
    // Player 1 Platforms
    //
    int nAdjust1 = nActivePlatforms - GetNumberActivePlatforms(ActiveLaserPlatforms1);
    
    if (nAdjust1 > 0)
    {
        for (int i=0; i<nAdjust1; ++i)
        {
            ActivatePlatform1();
        }
    }
    if (nAdjust1 < 0)
    {
        for (int i=0; i<-nAdjust1; ++i)
        {
            DeactivatePlatform1();
        }
    }

    //
    // Player 2 Platforms
    //
    int nAdjust2 = nActivePlatforms - GetNumberActivePlatforms(ActiveLaserPlatforms2);
    
    if (nAdjust2 > 0)
    {
        for (int i=0; i<nAdjust2; ++i)
        {
            ActivatePlatform2();
        }
    }
    if (nAdjust2 < 0)
    {
        for (int i=0; i<-nAdjust2; ++i)
        {
            DeactivatePlatform2();
        }
    }


}


void ALaserTennisGameModeBase::ActivatePlatform1()
{
    int nRestingPlatforms = ActiveLaserPlatforms1.Num() - GetNumberActivePlatforms(ActiveLaserPlatforms1);

    int32 RandomInt = FMath::RandRange(1, nRestingPlatforms);

    int index = 0;
    for (int i=0; i<ActiveLaserPlatforms1.Num(); ++i)
    {
        if (ActiveLaserPlatforms1[i] == 0)
        {
            index++;
        }
        
        if (index == RandomInt)
        {
            AActor* Actor = laserPlatforms1[i];
            ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
            if (Platform)
            {
                Platform->Activate();        
            }
            ActiveLaserPlatforms1[i] = 1;
        }

        break;
        
    }
}

void ALaserTennisGameModeBase::DeactivatePlatform1()
{
    
    int32 RandomInt = FMath::RandRange(1, GetNumberActivePlatforms(ActiveLaserPlatforms1));

    int index = 0;
    for (int i=0; i<ActiveLaserPlatforms1.Num(); ++i)
    {
        if (ActiveLaserPlatforms1[i] == 1)
        {
            index++;
        }
        
        if (index == RandomInt)
        {
            AActor* Actor = laserPlatforms1[i];
            ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
            if (Platform)
            {
                Platform->Deactivate();        
            }
            ActiveLaserPlatforms1[i] = 0;
        }

        break;
        
    }
}


void ALaserTennisGameModeBase::ActivatePlatform2()
{
    int nRestingPlatforms = ActiveLaserPlatforms2.Num() - GetNumberActivePlatforms(ActiveLaserPlatforms2);

    int32 RandomInt = FMath::RandRange(1, nRestingPlatforms);

    int index = 0;
    for (int i=0; i<ActiveLaserPlatforms2.Num(); ++i)
    {
        if (ActiveLaserPlatforms2[i] == 0)
        {
            index++;
        }
        
        if (index == RandomInt)
        {
            AActor* Actor = laserPlatforms2[i];
            ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
            if (Platform)
            {
                Platform->Activate();        
            }
            ActiveLaserPlatforms2[i] = 1;
        }

        break;
        
    }
}

void ALaserTennisGameModeBase::DeactivatePlatform2()
{
    
    int32 RandomInt = FMath::RandRange(1, GetNumberActivePlatforms(ActiveLaserPlatforms2));

    int index = 0;
    for (int i=0; i<ActiveLaserPlatforms2.Num(); ++i)
    {
        if (ActiveLaserPlatforms2[i] == 1)
        {
            index++;
        }
        
        if (index == RandomInt)
        {
            AActor* Actor = laserPlatforms2[i];
            ALaserActivationPlatform* Platform = Cast<ALaserActivationPlatform>(Actor);
            if (Platform)
            {
                Platform->Deactivate();        
            }
            ActiveLaserPlatforms2[i] = 0;
        }

        break;
        
    }
}


int ALaserTennisGameModeBase::GetNumberActivePlatforms(const TArray<int>& PlatformsMap) const
{
    int counter = 0;
    for (int k : PlatformsMap)
    {
        counter += k;
    }

    return counter;
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