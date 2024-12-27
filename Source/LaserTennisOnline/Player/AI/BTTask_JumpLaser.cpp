// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_JumpLaser.h"
#include "BasePlayer.h"

EBTNodeResult::Type UBTTask_JumpLaser::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ABasePlayer* AiPlayer = Cast<ABasePlayer>(OwnerComp.GetOwner());

    if (AiPlayer)
    {
        AiPlayer->Jump();

        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;
    }
}