// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonClicked);
	}

}

void UGameOverWidget::SetText(bool bGameWon)
{

	// ItemTitle can be nullptr if we haven't created it in the
	// Blueprint subclass

	if (GameOverText)
	{
		FText Text = FText::FromString(TEXT("GameOver \n Defeat"));
		FColor Color = FColor::Red;

		if (bGameWon)
		{
			Text = FText::FromString(TEXT("GameOver \n Victory"));
			Color = FColor::Green;
		}

		GameOverText->SetText(Text);
		GameOverText->SetColorAndOpacity(Color);
	}
	
}


void UGameOverWidget::MainMenuButtonClicked()
{
	if (MainMenuButton)
	{
		MainMenuButton->SetIsEnabled(false);
	}

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevel);
}