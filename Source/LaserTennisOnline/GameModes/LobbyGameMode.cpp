// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayer.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	
    if (NumberOfPlayers == 1)
	{
		InitiateStartGame();
	}
}


void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<TObjectPtr<APlayerState>> PlayerStateArray = GameState.Get()->PlayerArray;
	
	for (TObjectPtr<APlayerState> PlayerStatePtr : PlayerStateArray)
	{
		
		APlayerController* PlayerController = PlayerStatePtr->GetPlayerController();

		if (PlayerController)
		{
			ABasePlayer* BasePlayer = Cast<ABasePlayer>(PlayerController->GetPawn());

			if (BasePlayer)
			{
				BasePlayer->DisplayLobbyWidgets();
			}
		}

	}

}

void ALobbyGameMode::InitiateStartGame()
{
	
	UWorld* World = GetWorld();
	if (World)
	{	
		World->GetTimerManager().SetTimer(TimerHandle, this, 
		&ALobbyGameMode::StartGame, 3., false);	
	}	
		
	TArray<TObjectPtr<APlayerState>> PlayerStateArray = GameState.Get()->PlayerArray;
	
	for (TObjectPtr<APlayerState> PlayerStatePtr : PlayerStateArray)
	{
		
		APlayerController* PlayerController = PlayerStatePtr->GetPlayerController();

		if (PlayerController)
		{
			ABasePlayer* BasePlayer = Cast<ABasePlayer>(PlayerController->GetPawn());

			if (BasePlayer)
			{
				BasePlayer->DisplayCountdown();
			}
		}

	}
}

void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();
	
	if (World)
	{
		bUseSeamlessTravel = true;
		FString TravelString = GameLevel + TEXT("?listen");
		World->ServerTravel(TravelString);
	}
	
}