// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayer.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameOverWidget.h"

void ALocalMultiplayer::BeginPlay()
{
    Super::BeginPlay();

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

    // Unpossess & Destroy default Pawns

    SharedPlayerController = GetWorld()->GetFirstPlayerController();
    PlayerController2 = UGameplayStatics::CreatePlayer(GetWorld(),1);
    APawn* Pawn1 = SharedPlayerController->GetPawn();
    APawn* Pawn2 = PlayerController2->GetPawn();
    if (Pawn1)
    {
        SharedPlayerController->UnPossess();
        Pawn1->Destroy();
    }
    if (Pawn2)
    {
        PlayerController2->UnPossess();
        Pawn2->Destroy();
    }

    // Create Players Pawns

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
    }

    // Possess new Pawns

    if (Player1 and SharedPlayerController)
    {
        SharedPlayerController->Possess(Player1);
    }
    if (Player2 and PlayerController2)
    {
        PlayerController2->Possess(Player2);
    }

    // Set Shared Camera

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),ACameraActor::StaticClass(),
    "LocalMultiplayer",TempArray);

    if (TempArray.Num() > 0)
    {
        ACameraActor* SharedCamera = Cast<ACameraActor>(TempArray[0]);
        if (SharedCamera and SharedPlayerController and PlayerController2)
        {
            SharedPlayerController->SetViewTarget(SharedCamera);
            PlayerController2->SetViewTarget(SharedCamera);
        }   
    }

    // Enable Inputs
    HandleInputAssignment();

    StartCountdown();

}


void ALocalMultiplayer::HandleInputAssignment()
{
    if (Player1 and Player2 and SharedPlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		SharedPlayerController->GetLocalPlayer());

        if (InputSubSystem)
        {
            // Add Mapping Context
            InputSubSystem->AddMappingContext(this->LocalMultiplayer_InputMapContext, 0);
        }

        UEnhancedInputComponent* InputComponent1 = Cast<UEnhancedInputComponent>(Player1->InputComponent);
        UEnhancedInputComponent* InputComponent2 = Cast<UEnhancedInputComponent>(Player2->InputComponent);

        if (InputComponent1)
        {
            InputComponent1->BindAction(P1_MoveAction, ETriggerEvent::Triggered, Player1, &ABasePlayer::move);   
            InputComponent1->BindAction(P1_JumpAction, ETriggerEvent::Triggered, Player1, &ABasePlayer::jump);  
            InputComponent1->BindAction(P1_DodgeAction, ETriggerEvent::Triggered, Player1, &ABasePlayer::dodge);  

            InputComponent1->BindAction(P2_MoveAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::move);   
            InputComponent1->BindAction(P2_JumpAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::jump);  
            InputComponent1->BindAction(P2_DodgeAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::dodge);
            
            InputComponent1->BindAction(PauseGameAction, ETriggerEvent::Triggered, Player1, &ABasePlayer::pauseGame);
        }
    
    }
}

void ALocalMultiplayer::StartCountdown()
{
    Super::StartCountdown();

    if (Player1)
    {
        Player1->StartCountdown(CountdownTime);
    }

}

void ALocalMultiplayer::StartGame()
{
    Super::StartGame();

    if (Player1)
    {
        Player1->StartGame();
    }

}

void ALocalMultiplayer::HandleMatchHasEnded()
{
    if (Player1 and Player1->GameOverWidget)
    {
        Player1->GameOverWidget->RemoveFromParent();
    }
    if (Player2 and Player2->GameOverWidget)
    {
        Player2->GameOverWidget->RemoveFromParent();
    }
    
    UGameOverWidget* NewGameOverWidget;
    if (Player1 and Player1->bIsAlive())
    {
        NewGameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), Player1WinnerWidgetClass);
    }
    else
    {
        NewGameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), Player2WinnerWidgetClass);
    }
    if (NewGameOverWidget)
    {
        NewGameOverWidget->MenuSetup();
    }
}