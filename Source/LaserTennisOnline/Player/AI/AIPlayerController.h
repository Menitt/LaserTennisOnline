// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"


#include "AIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API AAIPlayerController : public AAIController
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float LaserDistanceTolerance = 0;

	class UBlackboardComponent* BlackboardComp;


private:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBehaviorTree* BehaviorTree;

	bool IsLaserNear() const;

};
