// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaScreen.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "CountDownWidget.h"
#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMegaScreen::AMegaScreen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Replication
	bReplicates = true;

	// 
	// COMPONENTS
	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	
	CountdownWidgetComp = CreateDefaultSubobject<UWidgetComponent>("Widget");
	CountdownWidgetComp->SetupAttachment(RootComponent);
	
	// Set default widget class (optional)
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/WBP_CountdownWidget.WBP_CountdownWidget_C")); // Replace with your widget's path
    if (WidgetClassFinder.Succeeded())
    {
        CountdownWidgetComp->SetWidgetClass(WidgetClassFinder.Class);
	}	

}

// Called when the game starts or when spawned
void AMegaScreen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMegaScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMegaScreen::StartCountdown(int Timer)
{
	
	UCountDownWidget* CountdownWidget = Cast<UCountDownWidget>(CountdownWidgetComp->GetUserWidgetObject());
	if (IsValid(CountdownWidget))
	{
		CountdownWidget->StartCountdown(Timer);
	
		ALaserTennisGameModeBase* GameMode = Cast<ALaserTennisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(GameMode))
		{
			CountdownWidget->OnCountdownComplete.AddDynamic(GameMode,&ALaserTennisGameModeBase::StartGame);
		}
	
	}
}