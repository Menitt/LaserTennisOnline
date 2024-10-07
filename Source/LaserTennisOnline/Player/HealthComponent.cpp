// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "LaserTennisGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "BasePlayer.h"
#include "CustomHUD.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	PlayerOwner = Cast<ABasePlayer>(GetOwner());
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage()
{
	

	if (Health <= 0)
	{
		return;
	}
	Health -= 1;

	// Update HUD
	APlayerController* PlayerController = Cast<APlayerController>(PlayerOwner->GetController());
	if (PlayerController)
	{
		ACustomHUD* GameHUD = Cast<ACustomHUD>(PlayerController->GetHUD());

		if (GameHUD)
		{
			GameHUD->UpdatePlayerHealth(Health);
		}
	}

	if (Health == 0)
	{
		HandleCharacterDeath();
	}
}


void UHealthComponent::HandleCharacterDeath()
{
	if (GEngine and PlayerOwner and PlayerOwner->GetLocalRole()==ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "HealthComponent->HandleCharacterDeath");
		ALaserTennisGameModeBase* GameMode = Cast<ALaserTennisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->GameOver();
		}
	}
}