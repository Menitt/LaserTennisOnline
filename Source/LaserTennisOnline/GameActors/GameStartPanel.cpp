// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartPanel.h"
#include "Components/StaticMeshComponent.h"
#include "GameStartPanelWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGameStartPanel::AGameStartPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;

	GameStartWidget = CreateDefaultSubobject<UWidgetComponent>("Widget");

	GameStartWidget->SetupAttachment(RootComponent);

	// Set default widget class (optional)
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/WBP_GameStartPanel.WBP_GameStartPanel_C")); // Replace with your widget's path
    if (WidgetClassFinder.Succeeded())
    {
        GameStartWidget->SetWidgetClass(WidgetClassFinder.Class);
    }
}

// Called when the game starts or when spawned
void AGameStartPanel::BeginPlay()
{
	Super::BeginPlay();

    TargetLocation = GetActorLocation() - FVector(0,0,1000);

}

// Called every frame
void AGameStartPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bShouldMove)
    {
        FVector NewLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), 
            TargetLocation, DeltaTime, Speed/10);
        SetActorLocation(NewLocation);
    }

    if ((GetActorLocation()-TargetLocation).Size() < 30)
    {
        Destroy();
    }

}

void AGameStartPanel::StartCountdown_Implementation()
{
	if (GameStartWidget)
    {
        UUserWidget* UserWidget = GameStartWidget->GetUserWidgetObject();
        if (UGameStartPanelWidget* CustomWidget = Cast<UGameStartPanelWidget>(UserWidget))
        {
            CustomWidget->StartCountdown(); 
            CustomWidget->PanelOwner = this;
        }
    }

    bShouldMove = true;

}

void AGameStartPanel::BroadcastGameStart()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        if (OnGameStarting.IsBound())
        {
            OnGameStarting.Broadcast();
        }
    }
}