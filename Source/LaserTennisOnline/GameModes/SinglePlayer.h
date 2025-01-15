// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserTennisGameModeBase.h"
#include "SinglePlayer.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API ASinglePlayer : public ALaserTennisGameModeBase
{
	GENERATED_BODY()
	

protected:

	virtual void PostLogin(class APlayerController* NewPlayer) override;


private:

	class APlayerController* PlayerController;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> AIControllerClass;

	class AAIPlayerController* AIPlayerController;

};
