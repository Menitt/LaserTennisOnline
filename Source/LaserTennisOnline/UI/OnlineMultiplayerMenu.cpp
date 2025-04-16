// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMultiplayerMenu.h"
#include "Components/Button.h"
#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h" 
#include "Sound/SoundWave.h"


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

    // Bind Sound File
	FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	Sound = LoadObject<USoundWave>(nullptr, *SoundPath);

    return true;
}


void UOnlineMultiplayerMenu::NativeDestruct()
{
    Super::NativeDestruct();
}

void UOnlineMultiplayerMenu::BackButtonClicked()
{
    PlayUISound();
    BackButton->SetIsEnabled(false);

    UMainMenu* MainMenu = CreateWidget<UMainMenu>(this, MainMenuWidgetClass);
    
    if (MainMenu)
    {
        RemoveFromParent();
        MainMenu->MenuSetup();
        
    }

}

void UOnlineMultiplayerMenu::PlayUISound()
{
    UWorld* World = GetWorld();
	bool ValidSound = World and Sound and Sound->IsValidLowLevel();

	if (ValidSound)
	{		
		UGameplayStatics::PlaySound2D(World, Sound, ScaleVolume, ScalePitch, StartTime);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("UOnlineMultiplayerMenu->PlayUISound: Could not play Sound due to bad Sound File!"));
		}
	}
}

void UOnlineMultiplayerMenu::HostButtonClicked()
{
    PlayUISound();
    Super::HostButtonClicked();
}

void UOnlineMultiplayerMenu::JoinButtonClicked()
{
    PlayUISound();
    Super::JoinButtonClicked();
}

void UOnlineMultiplayerMenu::FriendsButtonClicked()
{
    PlayUISound();
    Super::FriendsButtonClicked();
}
