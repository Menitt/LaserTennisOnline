// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_NoLaserNearLoop.h"
#include "Kismet/GameplayStatics.h"
#include "LaserRay.h"
#include "DrawDebugHelpers.h" 



UBTDecorator_NoLaserNearLoop::UBTDecorator_NoLaserNearLoop()
{
     bNotifyTick = true;
}

bool UBTDecorator_NoLaserNearLoop::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    AActor* AIPlayer = OwnerComp.GetOwner();

    if (AIPlayer)
    {
        TArray<AActor*> Lasers;

        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaserRay::StaticClass(), Lasers);

        for(AActor* Laser : Lasers)
        {
            float dist = (Laser->GetActorLocation() - AIPlayer->GetActorLocation()).Size();
            if (dist < DistanceLaserTolerance)
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }

    
}

void UBTDecorator_NoLaserNearLoop::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UE_LOG(LogTemp, Warning, TEXT("TickNode inside decorator"));

    TArray<AActor*> Lasers;

    AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());

    APawn* AIPlayer = nullptr;
    
    if(AIController)
    {
        AIPlayer = AIController->GetPawn();
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaserRay::StaticClass(), Lasers);
    if (AIPlayer)
    {
        for (AActor* Laser : Lasers)
        {
            DrawDebugLine(GetWorld(), AIPlayer->GetActorLocation(), Laser->GetActorLocation(), 
            FColor::Green, false, -1, 0, 5.);
        }
    }
    
}