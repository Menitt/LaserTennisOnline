// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTarget.h"
#include "Kismet/GameplayStatics.h"
#include "LaserActivationPlatform.h"
#include "Math/UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USelectTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if(BlackboardComp)
    {
        TArray<AActor*> Platforms;
        
        UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALaserActivationPlatform::StaticClass(), "2", Platforms);

        if(Platforms.Num()>0)
        {
            int32 RandomInt = FMath::RandRange(0, Platforms.Num()-1);
            
            BlackboardComp->SetValueAsVector("TestTarget", Platforms[RandomInt]->GetActorLocation());

            return EBTNodeResult::Succeeded;
        }
        else
        {
            return EBTNodeResult::Failed;
        }

    }
    else
    {
        return EBTNodeResult::Failed;
    }

}