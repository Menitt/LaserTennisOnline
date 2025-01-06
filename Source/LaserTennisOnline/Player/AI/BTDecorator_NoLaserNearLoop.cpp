// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_NoLaserNearLoop.h"
#include "Kismet/GameplayStatics.h"
#include "LaserRay.h"
#include "DrawDebugHelpers.h" 
#include "AIController.h"


UBTDecorator_NoLaserNearLoop::UBTDecorator_NoLaserNearLoop()
{
    bNotifyTick = true;
}

bool UBTDecorator_NoLaserNearLoop::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    APawn* AIPlayer = nullptr;

    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        // Return the controlled pawn (the AI player)
        AIPlayer =  AIController->GetPawn();
    }

    if (AIPlayer)
    {
        TArray<AActor*> Lasers;

        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaserRay::StaticClass(), Lasers);

        for (AActor* Laser : Lasers)
        {
            // Get the world transform matrix
            FTransform WorldTransform = Laser->GetActorTransform();
            
            FVector LocalPlayerLocation = WorldTransform.InverseTransformVector(AIPlayer->GetActorLocation()-Laser->GetActorLocation());

            float Y_coordinate = LocalPlayerLocation.Y;

            FVector PlayerProjection = WorldTransform.TransformVector(FVector(0,Y_coordinate,0)) + Laser->GetActorLocation();

            float Distance = (PlayerProjection - AIPlayer->GetActorLocation()).Size();

            UE_LOG(LogTemp, Warning, TEXT("Laser Distance: %f"), Distance);

            if (Distance < DistanceLaserTolerance)
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
    
    APawn* AIPlayer = nullptr;

    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        // Return the controlled pawn (the AI player)
        AIPlayer =  AIController->GetPawn();
    }

    TArray<AActor*> Lasers;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaserRay::StaticClass(), Lasers);
    if (AIPlayer)
    {
        for (AActor* Laser : Lasers)
        {
            
            // Get the world transform matrix
            FTransform WorldTransform = Laser->GetActorTransform();
            
            FVector LocalPlayerLocation = WorldTransform.InverseTransformVector(AIPlayer->GetActorLocation()-Laser->GetActorLocation());

            float Y_coordinate = LocalPlayerLocation.Y;

            FVector PlayerProjection = WorldTransform.TransformVector(FVector(0,Y_coordinate,0)) + Laser->GetActorLocation();

            DrawDebugLine(GetWorld(), AIPlayer->GetActorLocation(), PlayerProjection, 
            FColor::Green, false, -1, 0, 5.);
        }
    }
    
}