// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTDecorator_NoLaserNearLoop.generated.h"

/**
 * 
 */
UCLASS()
class LASERTENNISONLINE_API UBTDecorator_NoLaserNearLoop : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTDecorator_NoLaserNearLoop();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;



private:
	UPROPERTY(EditAnywhere)
	float DistanceLaserTolerance;





};
