// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartPanel.h"
#include "Components/StaticMeshComponent.h"
#include "GameStartPanelWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGameStartPanel::AGameStartPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

// Called every frame
void AGameStartPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameStartPanel::StartCountdown()
{
	if (GameStartWidget)
    {
        UUserWidget* UserWidget = GameStartWidget->GetUserWidgetObject();
        if (UGameStartPanelWidget* CustomWidget = Cast<UGameStartPanelWidget>(UserWidget))
        {
            CustomWidget->StartCountdown(); // Call your method
        }
    }
}

