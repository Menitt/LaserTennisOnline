// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CustomCharacterMovementComponent.h"
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.h"
#include "InGameMenu.h"
#include "BaseUserWidget.h"
#include "CountDownWidget.h"
#include "LaserTennisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"


#pragma region Constructor & Initialization

ABasePlayer::ABasePlayer(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Character Movement Component
	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	CustomCharacterMovementComponent->SetIsReplicated(true);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.f, 350.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 2000.f;
	JumpMaxCount = 2;

	// Camera Components
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MyCameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Heath Component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	
	// Audio Component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	AudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Let Character Movement Component handles Pawn rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Take Damage Delegate
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (SkeletalMesh)
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnTakeDamageMontageCompleted);
		}
	}

	// Fetch Sound Asset
	FString WalkSoundPath = SoundFolder + WalkSoundFile + "." + WalkSoundFile; 
	WalkSound = LoadObject<USoundCue>(nullptr, *WalkSoundPath);

	if (AudioComponent and WalkSound)
	{
		AudioComponent->SetSound(WalkSound);
	}

}

void ABasePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APlayerController* PlayerController = Cast<APlayerController>(NewController);

	if (PlayerController and PlayerController->IsLocalController())
	{
		EnableEnhancedInputSystem(PlayerController);
	}
}



#pragma endregion

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size();

}


#pragma region Input

void ABasePlayer::EnableEnhancedInputSystem(APlayerController* PlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
	PlayerController->GetLocalPlayer());

	if (SubSystem)
	{
		SubSystem->AddMappingContext(this->InputMapContext, 0);
	}
}
	

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* enhancedInputComponent = 
	CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, 
		this, &ABasePlayer::move);
		enhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Started, 
		this, &ABasePlayer::jump);
		enhancedInputComponent->BindAction(pauseGameAction, ETriggerEvent::Started, 
		this, &ABasePlayer::pauseGame);
		enhancedInputComponent->BindAction(dodgeAction, ETriggerEvent::Started, 
		CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::DashPressed);
	}


}

void ABasePlayer::move(const FInputActionValue &value)
{
	FVector2D moveValue = value.Get<FVector2D>();	
	AddMovementInput(FVector(1,0,0),moveValue.X);
	AddMovementInput(FVector(0,1,0),moveValue.Y);

	// Update Controller rotation so that character Rotates with while moving
	if (Controller)
	{
		Controller->SetControlRotation(FVector(moveValue.X,moveValue.Y,0).Rotation());
	}
}

void ABasePlayer::jump(const FInputActionValue& value)
{
	
	bool jumpNow = value.Get<bool>();
	if(jumpNow) 
	{
		// Animation for double jump
		if (JumpCurrentCount == 1)
		{
			this->PlayAnimMontage(DoubleJumpMontage);	
		}
		ACharacter::Jump();

	}
} 

void ABasePlayer::pauseGame(const FInputActionValue& value)
{
	// Spawn Menu Widget
	UInGameMenu* InGameMenu = CreateWidget<UInGameMenu>(GetWorld(),
	InGameMenuWidgetClass);

	if (InGameMenu)
	{
		InGameMenu->MenuSetup();
	}
}

void ABasePlayer::dodge(const FInputActionValue& value)
{
	if (CustomCharacterMovementComponent)
	{
		CustomCharacterMovementComponent->DashPressed();
	}
}


#pragma endregion


#pragma region Gameplay

void ABasePlayer::StartCountdown_Implementation(int Timer)
{
	if (IsLocallyControlled())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		UCountDownWidget* CountdownWidget = CreateWidget<UCountDownWidget>(GetWorld(),CountdownWidgetClass);

		UE_LOG(LogTemp, Warning, TEXT("Base Player -> Start Countdown"));

		// Spawn Countdown Widget
		if (CountdownWidget)
		{
			CountdownWidget->MenuSetup();
			CountdownWidget->StartCountdown(Timer);
			// If Actor is on the server, bind end of count down to Game mode start
			if (GetLocalRole() == ROLE_Authority)
			{
				ALaserTennisGameModeBase* GameMode = Cast<ALaserTennisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GameMode)
				{
					CountdownWidget->OnCountdownComplete.AddDynamic(GameMode,&ALaserTennisGameModeBase::StartGame);
				}
			}

		}

	}

}

void ABasePlayer::StartGame_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController and IsLocallyControlled())
	{
		EnableEnhancedInputSystem(PlayerController);
		this->EnableInput(PlayerController);
	}
}


void ABasePlayer::CustomTakeDamage_Implementation()
{

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		this->DisableInput(PlayerController);
	}

	// Animation
	this->PlayAnimMontage(TakeDamageMontage);
	

	if (GetLocalRole() == ROLE_Authority)
	{
		ALaserTennisGameModeBase* GameMode = Cast<ALaserTennisGameModeBase>(GetWorld()->GetAuthGameMode());
		// Update Health
		HealthComponent->TakeDamage();
		if (GameMode)
		{
			GameMode->UpdateHealthPanel();
			// Disable inputs for local multiplayer gamemode
			GameMode->DisablePlayerInput(this);
		}
	}

}


void ABasePlayer::GameOver_Implementation(bool bWonGame)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController and PlayerController->IsLocalController())
	{
		this->DisableInput(PlayerController);
	
		// Spawn Menu Widget
		if (bWonGame)
		{
			GameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(),GameOverVictoryClass);
		}
		else
		{
			GameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(),GameOverDefeatClass);
		}
		
		if (GameOverWidget)
		{
			GameOverWidget->MenuSetup();
		}
	}

	if (not bWonGame)
	{
		HandleDestruction();
	}

}


void ABasePlayer::HandleDestruction()
{
	GetMesh()->SetVisibility(false);

	// Destroy Pawn if AI controlled
	AAIController* AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		Destroy();
	}
}


void ABasePlayer::OnTakeDamageMontageCompleted(UAnimMontage* AnimMontage, bool bInterrupted)
{
	
	if (AnimMontage == TakeDamageMontage)
	{		
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (PlayerController)
		{
			this->EnableInput(PlayerController);
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		ALaserTennisGameModeBase* GameMode = Cast<ALaserTennisGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			// Enable Input for local multiplayer Game Mode
			GameMode->EnablePlayerInput(this);
		}
	}

}


#pragma endregion


#pragma region Sound

void ABasePlayer::SetWalkSound(float Speed)
{
	// Clear previous timer
	GetWorld()->GetTimerManager().ClearTimer(WalkSoundTimerHandle);
	// Set new timer
	float Ratio = WalkSoundConstantRate/Speed;
	
	PlayWalkSound();
	// PlayWalkSound();
	GetWorld()->GetTimerManager().SetTimer(WalkSoundTimerHandle,this,&ThisClass::PlayWalkSound,Ratio,true);
}

void ABasePlayer::PlayWalkSound()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->Play();
	}
}


#pragma endregion