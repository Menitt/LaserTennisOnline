// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownWidget.h"
#include "Runtime/UMG/Public/Components/EditableTextBox.h"
#include "Runtime/UMG/Public/Components/MultiLineEditableTextBox.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Styling/SlateColor.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateTypes.h"


void UCountDownWidget::StartCountdown(int Time)
{
    UWorld* World = GetWorld();
    if (World)
    {
        // Change Text Settings
        FTextBlockStyle TextStyle;

        FSlateFontInfo FontInfo;

        // Load your font asset (must be a UFont)
        FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/UI/fonts/ASTERA_v2_Font.ASTERA_v2_Font")); // Path to .ufont
        FontInfo.TypefaceFontName = FName("Regular");  // "Regular", "Italic", etc. must exist in the font asset
        FontInfo.Size = FontSize;
        TextStyle.Font = FontInfo;
        // Set Color
        TextStyle.ColorAndOpacity = FLinearColor(TextColorRGB.X, TextColorRGB.Y, TextColorRGB.Z);
        // TextStyle.ColorAndOpacity = FSlateColor(FLinearColor::Green); // or any color

        World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateCountdown, 1.0f, true);
        
        if (CountdownText)
        {       
            // Apply style
            CountdownText->SetTextStyle(TextStyle);
            CountdownText->SetText(FText::AsNumber(Time));
        }

        Counter = Time;

        PlayerController =  World->GetFirstPlayerController();

    }
    
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
    // RemoveWidget (GameMode = OnlineMult or Singleplayer -> Different widget blueprint)
    if (not IsPermanent)
    {
        MenuTearDown();
    }
    else
    {
        // Update Text
        FTextBlockStyle TextStyle;

        FSlateFontInfo FontInfo;

        // Load your font asset (must be a UFont)
        FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/UI/fonts/ASTERA_v2_Font.ASTERA_v2_Font")); // Path to .ufont 
        FontInfo.TypefaceFontName = FName("Regular");  // "Regular", "Italic", etc. must exist in the font asset
        FontInfo.Size = 100;
        TextStyle.Font = FontInfo;
        // Set Color
        // TextStyle.ColorAndOpacity = FSlateColor(FLinearColor::Green); // or any color
        TextStyle.ColorAndOpacity = FLinearColor(TextColorRGB.X, TextColorRGB.Y, TextColorRGB.Z);

        if (CountdownText)
        {       
            // Apply style
            CountdownText->SetTextStyle(TextStyle);
            CountdownText->SetText(FText::FromString("Game On!"));
        }
    }
    

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