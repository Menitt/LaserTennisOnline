// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserTennisGameModeBase.h"
#include "OnlineMultiplayer.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API AOnlineMultiplayer : public ALaserTennisGameModeBase
{
	GENERATED_BODY()

	virtual void PostLogin(class APlayerController* NewPlayer) override;



};