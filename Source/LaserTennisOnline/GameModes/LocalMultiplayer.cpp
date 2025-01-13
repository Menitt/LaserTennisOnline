// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayer.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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

    PlayerController1 = GetWorld()->GetFirstPlayerController();
    PlayerController2 = UGameplayStatics::CreatePlayer(GetWorld(),1);
    APawn* Pawn1 = PlayerController1->GetPawn();
    APawn* Pawn2 = PlayerController2->GetPawn();
    if (Pawn1)
    {
        PlayerController1->UnPossess();
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

    if (Player1 and PlayerController1)
    {
        PlayerController1->Possess(Player1);
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
        if (SharedCamera and PlayerController1 and PlayerController2)
        {
            PlayerController1->SetViewTarget(SharedCamera);
            PlayerController2->SetViewTarget(SharedCamera);
        }   
    }

    // Enable Inputs
    HandleInputAssignment();

}


void ALocalMultiplayer::HandleInputAssignment()
{
    if (Player1 and Player2 and PlayerController1)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController1->GetLocalPlayer());

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

        // if (InputComponent2)
        // {
        //     InputComponent2->BindAction(P2_MoveAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::move);   
        //     InputComponent2->BindAction(P2_JumpAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::jump);  
        //     InputComponent2->BindAction(P2_DodgeAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::dodge);
        // }
    
    }
}