// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPanel.h"
#include "Components/StaticMeshComponent.h"
#include "GameStartPanelWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthWidget.h"

// Sets default values
AHealthPanel::AHealthPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("Widget");

	HealthWidget->SetupAttachment(RootComponent);

	// Set default widget class (optional)
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/WBP_HealthWidget.WBP_HealthWidget_C")); // Replace with your widget's path
    if (WidgetClassFinder.Succeeded())
    {
        HealthWidget->SetWidgetClass(WidgetClassFinder.Class);
    }

}

// Called when the game starts or when spawned
void AHealthPanel::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation() + FVector(0,0,zOffset);
	
}

// Called every frame
void AHealthPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldMove)
    {
        FVector NewLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), 
            TargetLocation, DeltaTime, Speed/10);
        SetActorLocation(NewLocation);
    }

	if ((GetActorLocation()-TargetLocation).Size() < 10)
    {
        bShouldMove = false;
    }

}

void AHealthPanel::UpdateWidgetHealth_Implementation(int PlayerHealth)
{
	UUserWidget* UserWidget = HealthWidget->GetUserWidgetObject();
	if (UHealthWidget* CustomWidget = Cast<UHealthWidget>(UserWidget))
	{
		CustomWidget->UpdateHealthDisplayed(PlayerHealth);
	}
}

void AHealthPanel::Activate_Implementation()
{
	bShouldMove = true;
}