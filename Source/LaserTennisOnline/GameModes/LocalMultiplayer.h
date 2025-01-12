// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserTennisGameModeBase.h"
#include "LocalMultiplayer.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ALocalMultiplayer : public ALaserTennisGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	

};
