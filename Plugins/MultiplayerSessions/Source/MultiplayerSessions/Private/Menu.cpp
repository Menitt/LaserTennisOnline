// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "GameFramework/PlayerController.h"  // For APlayerController
#include "Engine/LocalPlayer.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	// bIsFocusable = true;
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
	}
	if (TestButton)
	{
		TestButton->OnClicked.AddDynamic(this, &ThisClass::TestButtonClicked);
	}
	if (FriendsButton)
	{
		FriendsButton->OnClicked.AddDynamic(this, &ThisClass::FriendsButtonClicked);
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				FString(TEXT("Successfully Created Session!"))
			);
		}
	
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		HostButton->SetIsEnabled(true);
	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Inside UMenu::OnFindSessions: trying to join session"))
			);
		}	
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}


void UMenu::QuitButtonClicked()
{
	QuitButton->SetIsEnabled(false);

	if (GEngine)
    {
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }
}

void UMenu::TestButtonClicked()
{
	TestButton->SetIsEnabled(false);

	if (GEngine)
    {	
		UGameplayStatics::OpenLevel(GetWorld(), TestLevel);
    }
}

void UMenu::FriendsButtonClicked()
{
	FriendsButton->SetIsEnabled(false);

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();



	if (OnlineSubsystem)
	{
		IOnlineFriendsPtr FriendsInterface = OnlineSubsystem->GetFriendsInterface();
        // IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (FriendsInterface.IsValid() and PlayerController)
        {
			int32 UserId = PlayerController->GetLocalPlayer()->GetUniqueID();
			// Assuming the player is logged in
			// const FUniqueNetIdPtr UserId = IdentityInterface->GetUniquePlayerId(0);;
			FriendsInterface->ReadFriendsList(UserId, TEXT("default"), FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnReadFriendsListComplete));
		}
	}
}


void UMenu::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	UWorld* World = GetWorld();
	
	if (bWasSuccessful and World)
    {
		APlayerController* PlayerController = World->GetFirstPlayerController();
        if (PlayerController)
		{
			// Process friends list
			TArray<TSharedRef<FOnlineFriend>>  Friends;
			IOnlineFriendsPtr FriendsInterface = IOnlineSubsystem::Get()->GetFriendsInterface();
			FriendsInterface->GetFriendsList(PlayerController->GetLocalPlayer()->GetUniqueID(), ListName, Friends);

			if (Friends.Num() == 0)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "No Friends Found");
				}
			}
			
			for (const TSharedRef<FOnlineFriend>& Friend : Friends)
			{
				if (GEngine)
				{
					FString FriendName = *Friend->GetDisplayName();
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green, FriendName);
				}
			}

		} 
    }
    else
    {
        if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "No Friends Found");
		}
    }
}


void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
