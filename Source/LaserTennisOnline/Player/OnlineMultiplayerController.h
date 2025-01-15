// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineMultiplayerController.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API AOnlineMultiplayerController : public APlayerController
{
	GENERATED_BODY()

	AOnlineMultiplayerController();

protected:
	virtual void BeginPlay() override;

};
