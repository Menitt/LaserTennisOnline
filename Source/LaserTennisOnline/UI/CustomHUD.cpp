// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomHUD.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

void ACustomHUD::DrawHUD()
{
    Super::DrawHUD();

    // Get the viewport size
    FVector2D ViewportSize;
    if (GEngine)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }
    

    // Set up the position for the health text
    const float TextX = ScreenLeftOffset; // pixels from the left
    const float TextY = ViewportSize.Y - ScreenBotOffset; // pixels from the bottom

    // Convert health to string
    FString HealthText = FString::Printf(TEXT("Health: %d"), PlayerHealth);

    // Draw the health text
    FCanvasTextItem TextItem(FVector2D(TextX, TextY), FText::FromString(HealthText), GEngine->GetMediumFont(), FLinearColor::White);
    TextItem.Scale = FontSize;
    Canvas->DrawItem(TextItem);

}

void ACustomHUD::UpdatePlayerHealth(int NewHealth)
{
    PlayerHealth = NewHealth;
}

void ACustomHUD::BeginPlay()
{
    Super::BeginPlay();

}