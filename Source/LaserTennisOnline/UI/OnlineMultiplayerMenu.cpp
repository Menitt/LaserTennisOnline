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

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    
    if (PlayerController)
    {	
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn and Sound)
		{
			FVector SoundLocation = PlayerPawn->GetActorLocation(); // You can set a custom location
			
			UGameplayStatics::PlaySoundAtLocation(this, Sound, SoundLocation,ScaleVolume,ScalePitch,StartTime);
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
