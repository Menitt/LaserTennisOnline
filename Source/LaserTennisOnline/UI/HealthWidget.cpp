// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "Runtime/UMG/Public/Components/EditableTextBox.h"

void UHealthWidget::UpdateHealthDisplayed(int Health)
{
    if (Text)
    {
        Text->SetText(FText::FromString(TextToDisplay + FString::FromInt(Health)));
    }
}

void UHealthWidget::InitializeText(const FString& InitialText, int PlayerHealth)
{
    
    TextToDisplay = InitialText;
    
    UpdateHealthDisplayed(PlayerHealth);
}