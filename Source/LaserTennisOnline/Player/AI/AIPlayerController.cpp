// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LaserRay.h"
#include "Kismet/GameplayStatics.h"



void AAIPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }

    BlackboardComp = GetBlackboardComponent();

    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector("TestTarget", FVector(-29812.506619,109.306837,672.245747));
    }

}


void AAIPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (IsLaserNear() and BlackboardComp)
    {
        BlackboardComp->SetValueAsBool("LaserAlert",true);
    }
    else
    {
        BlackboardComp->ClearValue("LaserAlert");
    }
}



bool AAIPlayerController::IsLaserNear() const
{
    TArray<AActor*> Lasers;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaserRay::StaticClass(), Lasers);
    
    bool Cond = false;

    for (AActor* Laser : Lasers)
    {
        ALaserRay* LaserRay = Cast<ALaserRay>(Laser);

        APawn* AIPlayer = GetPawn();

        if (LaserRay and AIPlayer and LaserRay->IsTargetAhead(AIPlayer->GetActorLocation()))
        {
            if (LaserRay->CalculateDistance(AIPlayer->GetActorLocation()) < LaserDistanceTolerance)
            {
                Cond = true;
            }
        }
    }

    return Cond;

}