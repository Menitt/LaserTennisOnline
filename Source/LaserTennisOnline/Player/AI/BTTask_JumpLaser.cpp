// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_JumpLaser.h"
#include "BasePlayer.h"
#include "AIController.h"





EBTNodeResult::Type UBTTask_JumpLaser::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ABasePlayer* AIPlayer = nullptr;

    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        // Return the controlled pawn (the AI player)
        AIPlayer =  Cast<ABasePlayer>(AIController->GetPawn());
    }
    

    if (AIPlayer)
    {
        AIPlayer->Jump();

        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;
    }
}