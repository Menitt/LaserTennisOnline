// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


void AAIPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }

    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector("TestTarget", FVector(-29812.506619,109.306837,672.245747));
    }

}