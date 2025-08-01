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


void ALocalMultiplayer::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

}



void ALocalMultiplayer::BeginPlay()
{
    Super::BeginPlay();

    // Store first player controller
    SharedPlayerController = GetWorld()->GetFirstPlayerController();
    // Create Player Controllers
    PlayerController1 = UGameplayStatics::CreatePlayer(GetWorld(), 1);
    PlayerController2 = UGameplayStatics::CreatePlayer(GetWorld(), 2);

    // Destroy Default Controller Pawn
    APawn* DefaultPawn = SharedPlayerController->GetPawn();
    if (DefaultPawn)
    {
        SharedPlayerController->UnPossess();
        DefaultPawn->Destroy();
    }

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

    // Create Players Pawns and Controllers
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


    // Possess new Pawns and destroy default pawns
    if (PlayerController1)
    {
        APawn* DummyPawn1 = PlayerController1->GetPawn();
        if (DummyPawn1)
        {
            DummyPawn1->Destroy();            
        }
        PlayerController1->Possess(Player1);
    }

    if (PlayerController2)
    {
        APawn* DummyPawn2 = PlayerController2->GetPawn();
        if (DummyPawn2)
        {
            DummyPawn2->Destroy();            
        }
        PlayerController2->Possess(Player2);
    }

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ABasePlayer::StaticClass(), "SharedPawn", TempArray);
    if (TempArray.Num() > 0)
    {
        SharedInputPawn = Cast<ABasePlayer>(TempArray[0]);
    }
    if (SharedPlayerController)
    {
        SharedPlayerController->Possess(SharedInputPawn);
    }

    // Set Shared Camera View
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),ACameraActor::StaticClass(),
    "LocalMultiplayer",TempArray);

    if (TempArray.Num() > 0)
    {
        ACameraActor* SharedCamera = Cast<ACameraActor>(TempArray[0]);
        if (SharedCamera and SharedPlayerController)
        {
            SharedPlayerController->SetViewTarget(SharedCamera);
        }   
    }

    // Enable Inputs
    HandleInputAssignment();

    FTimerHandle StartGameTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &ThisClass::StartCountdown, .5f, false);

}


void ALocalMultiplayer::HandleInputAssignment()
{

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("LocalMultiplayer->HandleInputAssignment"));
    }
    
    if (Player1 and Player2 and SharedInputPawn and SharedPlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		SharedPlayerController->GetLocalPlayer());

        if (InputSubSystem)
        {
            // Add Mapping Context
            InputSubSystem->AddMappingContext(this->LocalMultiplayer_InputMapContext, 0);  
        }
    
    }

    SharedInputComponent = Cast<UEnhancedInputComponent>(SharedInputPawn->InputComponent);

    if (SharedInputComponent)
    {
        P1_MoveHandle = SharedInputComponent->BindAction(P1_MoveAction, ETriggerEvent::Triggered, Player1, &ABasePlayer::move).GetHandle();   
        P1_JumpHandle = SharedInputComponent->BindAction(P1_JumpAction, ETriggerEvent::Started, Player1, &ABasePlayer::jump).GetHandle();  
        P1_DodgeHandle = SharedInputComponent->BindAction(P1_DodgeAction, ETriggerEvent::Started, Player1, &ABasePlayer::dodge).GetHandle();  

        P2_MoveHandle = SharedInputComponent->BindAction(P2_MoveAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::move).GetHandle();   
        P2_JumpHandle = SharedInputComponent->BindAction(P2_JumpAction, ETriggerEvent::Started, Player2, &ABasePlayer::jump).GetHandle();  
        P2_DodgeHandle = SharedInputComponent->BindAction(P2_DodgeAction, ETriggerEvent::Started, Player2, &ABasePlayer::dodge).GetHandle();
        
        SharedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, Player1, &ABasePlayer::pauseGame);
    }

}

void ALocalMultiplayer::StartCountdown()
{
    Super::StartCountdown();

    if (IsValid(SharedInputPawn) and not TestingFlag)
    {
        SharedInputPawn->StartCountdown(CountdownTime);
        UE_LOG(LogTemp, Warning, TEXT("LocalMultiplayer->StartCountdown, testing not active"));
    }

}

void ALocalMultiplayer::StartGame()
{
    Super::StartGame();

    if (SharedInputPawn)
    {
        SharedInputPawn->StartGame();
    }

}

void ALocalMultiplayer::HandleMatchHasEnded()
{
    
    Super::HandleMatchHasEnded();
    // if (Player1 and Player1->GameOverWidget)
    // {
    //     Player1->GameOverWidget->RemoveFromParent();
    // }
    // if (Player2 and Player2->GameOverWidget)
    // {
    //     Player2->GameOverWidget->RemoveFromParent();
    // }
    
    // UGameOverWidget* NewGameOverWidget;
    // if (Player1 and Player1->bIsAlive())
    // {
    //     NewGameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), Player1WinnerWidgetClass);
    // }
    // else
    // {
    //     NewGameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), Player2WinnerWidgetClass);
    // }
    // if (NewGameOverWidget)
    // {
    //     NewGameOverWidget->MenuSetup();
    // }
}


void ALocalMultiplayer::RemovePlayers()
{
    
    if (PlayerController1)
    {
        Logout(PlayerController1);
        RemovePlayerControllerFromPlayerCount(PlayerController1);
        PlayerController1->UnPossess();
        PlayerController1->Destroy();
    }
    if (PlayerController2)
    {
        Logout(PlayerController2);
        RemovePlayerControllerFromPlayerCount(PlayerController2);
        PlayerController2->UnPossess();
        PlayerController2->Destroy();
    }

}

void ALocalMultiplayer::ReturnToMainMenuHost()
{  
    
    // Remove Local Players
    ULocalPlayer* LocalPlayer1 = GetGameInstance()->GetLocalPlayerByIndex(1);
    ULocalPlayer* LocalPlayer2 = GetGameInstance()->GetLocalPlayerByIndex(2);
    if (LocalPlayer1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Removing player1"));
        GetGameInstance()->RemoveLocalPlayer(LocalPlayer1);
    }
    if (LocalPlayer2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Removing players2"));
        GetGameInstance()->RemoveLocalPlayer(LocalPlayer2);
    }
    
    Super::ReturnToMainMenuHost();

    
}

void ALocalMultiplayer::DisablePlayerInput(ABasePlayer* Player)
{
    if (SharedInputComponent)
    {
        if (Player == Player1)
        {
            SharedInputComponent->RemoveBindingByHandle(P1_MoveHandle);
            SharedInputComponent->RemoveBindingByHandle(P1_JumpHandle);
            SharedInputComponent->RemoveBindingByHandle(P1_DodgeHandle);
        }
        else if (Player == Player2)
        {
            SharedInputComponent->RemoveBindingByHandle(P2_MoveHandle);
            SharedInputComponent->RemoveBindingByHandle(P2_JumpHandle);
            SharedInputComponent->RemoveBindingByHandle(P2_DodgeHandle);
        }
    }
}

void ALocalMultiplayer::EnablePlayerInput(ABasePlayer* Player)
{
    if (SharedInputComponent)
    {
        if (Player == Player1)
        {
            P1_MoveHandle = SharedInputComponent->BindAction(P1_MoveAction, ETriggerEvent::Triggered, Player1, &ABasePlayer::move).GetHandle();   
            P1_JumpHandle = SharedInputComponent->BindAction(P1_JumpAction, ETriggerEvent::Started, Player1, &ABasePlayer::jump).GetHandle();  
            P1_DodgeHandle = SharedInputComponent->BindAction(P1_DodgeAction, ETriggerEvent::Started, Player1, &ABasePlayer::dodge).GetHandle();  
        }
        else if (Player == Player2)
        {
            P2_MoveHandle = SharedInputComponent->BindAction(P2_MoveAction, ETriggerEvent::Triggered, Player2, &ABasePlayer::move).GetHandle();   
            P2_JumpHandle = SharedInputComponent->BindAction(P2_JumpAction, ETriggerEvent::Started, Player2, &ABasePlayer::jump).GetHandle();  
            P2_DodgeHandle = SharedInputComponent->BindAction(P2_DodgeAction, ETriggerEvent::Started, Player2, &ABasePlayer::dodge).GetHandle();
        }
    }
}