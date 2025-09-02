// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "CeilingLight.h"
#include "Math/UnrealMathUtility.h"


AMainMenuGameMode::AMainMenuGameMode()
{
    bUseSeamlessTravel = true;
}


void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
}

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* Player = GetWorld()->GetFirstPlayerController();

    //
    // Set Menu Camera View and Default Pawn Location
    //
    TArray<AActor*> TempArray;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),ACameraActor::StaticClass(),
    "MainMenu",TempArray);

    if (TempArray.Num() > 0 and IsValid(Player))
    {
        ACameraActor* MainMenuCamera = Cast<ACameraActor>(TempArray[0]);
        if (IsValid(MainMenuCamera))
        {
            Player->SetViewTarget(MainMenuCamera);
            APawn* DefaultPawn = Player->GetPawn();
            if (IsValid(DefaultPawn))
            {
                DefaultPawn->SetActorLocation(MainMenuCamera->GetActorLocation());
            }
        }   
    }
    
    //
    // Create Main Menu Widget
    //
    UMainMenu* MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuWidgetClass);

    if (MainMenuWidget)
    {
        MainMenuWidget->MenuSetup();
    }

    // 
    // Lighting
    // 
    FetchLightActors();
    GetWorld()->GetTimerManager().SetTimer(LightTimerHandle, this, &ThisClass::ActivateLights, 0.5f, true);

}

void AMainMenuGameMode::FetchLightActors()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACeilingLight::StaticClass(), CeilingLights);
}

void AMainMenuGameMode::ActivateLights()
{

    int ColorIdx = FMath::RandRange(0,ColorList.Num()-1);
    FLinearColor NewColor = ColorList[ColorIdx];

    int LightToActivate = FMath::RandRange(MinLightActivate, MAxLightActivate-1);

    for (int i=0; i<LightToActivate; ++i)
    {
        int LightIdx = FMath::RandRange(0,CeilingLights.Num()-1);

        ACeilingLight* Light = Cast<ACeilingLight>(CeilingLights[LightIdx]);

        if (IsValid(Light))
        {
            Light->ChangeLightColor(NewColor);
        }
    }
}