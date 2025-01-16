// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMultiplayerMenu.h"
#include "Components/Button.h"
#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h" 


bool UOnlineMultiplayerMenu::Initialize()
{
    if (!Super::Initialize())
	{
		return false;
	}

	if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
    }

	return true;

    UE_LOG(LogTemp, Warning, TEXT("Online Menu Initialize"));
}


void UOnlineMultiplayerMenu::NativeDestruct()
{
    Super::NativeDestruct();

    UE_LOG(LogTemp, Warning, TEXT("Online Menu Custom Destructor!"));
}

void UOnlineMultiplayerMenu::BackButtonClicked()
{
    BackButton->SetIsEnabled(false);

    UMainMenu* MainMenu = CreateWidget<UMainMenu>(this, MainMenuWidgetClass);
    
    if (MainMenu)
    {
        RemoveFromParent();
        MainMenu->MenuSetup();
        
    }

}


