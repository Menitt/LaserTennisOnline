// Fill out your copyright notice in the Description page of Project Settings.


#include "Spark.h"
#include "Components\StaticMeshComponent.h"
#include "Components\TimelineComponent.h"
#include "Components\SplineComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ASpark::ASpark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
    // Mesh
    //
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    RootComponent = Mesh;

    //
    // Niagara
    //
    Niagara = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
    Niagara->SetupAttachment(RootComponent);

    // 
    // Sound
    //
    AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio Component");
    AudioComponent->SetupAttachment(RootComponent);

    // 
    // Timeline
    //
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
		// Timeline->SetPlayRate(1.0f / Period);
        Timeline->PlayFromStart();
    }

    // //
    // // Audio Component
    // //

    // FString SoundPath = SoundFolder + SoundFile + "." + SoundFile;
	// USoundWave* Sound = LoadObject<USoundWave>(nullptr, *SoundPath); 

    // if (IsValid(AudioComponent) and IsValid(Sound) and Sound->IsValidLowLevel())
    // {
    //     AudioComponent->SetSound(Sound);
	// 	AudioComponent->Play();
    // }

}


void ASpark::HandleProgress(float Value)
{
    if (IsValid(Spline) and IsValid(Timeline))
	{
		float SplineLength = Spline->GetSplineLength();
		float DistanceAlongSpline = Value * SplineLength;

        // Set Period for timeline
        float Period = SplineLength / Speed / 100;
        Timeline->SetPlayRate(1.0f / Period);

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
            
            Destroy();
        }
	}

}


// Called every frame
void ASpark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASpark::SetSpline(USplineComponent* PlatformSpline)
{
	Spline = PlatformSpline;
}

void ASpark::SetActivePlayer(int Player)
{
    ActivePlayer = Player;
}