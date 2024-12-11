// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartPanelWidget.h"
#include "Runtime/UMG/Public/Components/EditableTextBox.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "GameStartPanel.h"


void UGameStartPanelWidget::StartCountdown()
{
    // Start a 5-second timer
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &ThisClass::UpdateCountdown, 1.0f, true);
}

void UGameStartPanelWidget::StartGame()
{
    if (PanelOwner)
    {
        PanelOwner->BroadcastGameStart();
    }
}


void UGameStartPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GameStartCountdown)
    {
        FText Text = FText::FromString("Waiting For Other Player");

        GameStartCountdown->SetText(Text);
    }

    Counter = -1;
}

void UGameStartPanelWidget::UpdateCountdown()
{
    if (Counter < 0)
    {
        FText Text = FText::FromString("Players Found");
        if (GameStartCountdown)
        {
            GameStartCountdown->SetText(Text);
        }
        Counter++;   
    }
    
    
    else if (Counter == Countdown)
    {
        FText Text = FText::FromString("Game Starting!!");
        if (GameStartCountdown)
        {
            GameStartCountdown->SetText(Text);
        }
        StartGame();
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }
    else 
    {
        FText Text = FText::FromString("Game Starts in " + FString::FromInt(Countdown-Counter));
        if (GameStartCountdown)
        {
            GameStartCountdown->SetText(Text);
        }
        
        Counter++;
    }
    
}