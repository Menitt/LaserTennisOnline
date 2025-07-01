// Fill out your copyright notice in the Description page of Project Settings.


#include "Spark.h"
#include "Components\StaticMeshComponent.h"
#include "Components\TimelineComponent.h"
#include "Components\SplineComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASpark::ASpark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
	
}

// Called when the game starts or when spawned
void ASpark::BeginPlay()
{
	Super::BeginPlay();
	
	if (MovementCurve and Timeline)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("HandleProgress"));

        FOnTimelineEvent TimelineFinished;
        TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

        Timeline->AddInterpFloat(MovementCurve, TimelineProgress);
        Timeline->SetTimelineFinishedFunc(TimelineFinished);
        Timeline->SetLooping(false);
		Timeline->SetPlayRate(1.0f / Period);
        Timeline->PlayFromStart();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SplineFollower: MovementCurve not assigned!"));
    }

}


void ASpark::HandleProgress(float Value)
{
    if (IsValid(Spline))
	{
		float SplineLength = Spline->GetSplineLength();
		float DistanceAlongSpline = Value * SplineLength;

		FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator NewRotation = Spline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

		this->SetActorLocationAndRotation(NewLocation, NewRotation);
	}
	
	
}

void ASpark::OnTimelineFinished()
{
    // Optional: Restart, reverse, or trigger event
    

    if (GetLocalRole() == ROLE_Authority)
	{
		if (OnSparkArrived.IsBound())
		{
			OnSparkArrived.Broadcast(ActivePlayer); // Trigger the delegate
		
            UE_LOG(LogTemp, Warning, TEXT("ASpark: Broadcasting!."));
        }
	}

}


// Called every frame
void ASpark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), 50, 20, FColor::Red, false, -1);

}


void ASpark::SetSpline(USplineComponent* PlatformSpline)
{
	Spline = PlatformSpline;
}

void ASpark::SetActivePlayer(int Player)
{
    ActivePlayer = Player;
}