// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownWidget.h"
#include "Runtime/UMG/Public/Components/EditableTextBox.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"



void UCountDownWidget::StartCountdown(int Time)
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateCountdown, 1.0f, true);

    if (CountdownText)
    {       
        CountdownText->SetText(FText::AsNumber(Time));
    }

    Counter = Time;

    PlayerController = GetWorld()->GetFirstPlayerController();

    PlayUISound();
}

void UCountDownWidget::UpdateCountdown()
{
    if (Counter == 0)
    {
        StartGame();
    }
    else
    {
        Counter = Counter-1;
        if (CountdownText)
        {       
            CountdownText->SetText(FText::AsNumber(Counter));
        }
    }
} 

void UCountDownWidget::StartGame()
{
    // Clean Timer
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    // Broadcast Game Starting
    BroadcastGameStart();
    // RemoveWidget
    MenuTearDown();
}

void UCountDownWidget::BroadcastGameStart()
{

    if (OnCountdownComplete.IsBound())
    {
        OnCountdownComplete.Broadcast();
    }
}

void UCountDownWidget::MenuSetup()
{
    AddToViewport();
}