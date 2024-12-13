// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "Runtime/UMG/Public/Components/EditableTextBox.h"

void UHealthWidget::UpdateHealthDisplayed(int Health)
{
    FText Text = FText::FromString("Health Points: " + FString::FromInt(Health));

    if (PlayerHealth)
    {
        PlayerHealth->SetText(Text);
    }

    UE_LOG(LogTemp, Warning, TEXT("Setting Text"));
}