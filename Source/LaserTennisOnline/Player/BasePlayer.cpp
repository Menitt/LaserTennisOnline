// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CustomCharacterMovementComponent.h"
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.h"
#include "InGameMenu.h"
// #include "OnlineSubsystem.h"

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
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1600.0f;
	CameraBoom->bUsePawnControlRotation = false;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	// Heath Component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");


}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	// Setting up Enhanced Input
	if(APlayerController* playerController = Cast<APlayerController>(this->GetController())){
		if(UEnhancedInputLocalPlayerSubsystem* subSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(playerController->GetLocalPlayer())){
			subSystem->AddMappingContext(this->inputMapContext, 0);
		}
	}

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
			UE_LOG(LogTemp, Warning, TEXT("ABasePlayer::BeginPlay -> Binding Delegate"));
			AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnTakeDamageMontageCompleted);
		}
	}


}

#pragma endregion

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#pragma region Input

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

	if (GetNetMode() == NM_ListenServer and GEngine and GetLocalRole() == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(12,2,FColor::Blue,"ABasePlayer->jump");
	}



} 

void ABasePlayer::pauseGame(const FInputActionValue& value)
{
	// Spawn Menu Widget
	UInGameMenu* InGameMenu =CreateWidget<UInGameMenu>(GetWorld(),
	InGameMenuWidgetClass);

	if (InGameMenu)
	{
		InGameMenu->MenuSetup();
	}
}


#pragma endregion


#pragma region Gameplay

void ABasePlayer::CustomTakeDamage_Implementation()
{

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		this->DisableInput(PlayerController);
	}


	// Animation
	this->PlayAnimMontage(TakeDamageMontage);
	
	// Update Health
	HealthComponent->TakeDamage();
}


void ABasePlayer::GameOver_Implementation(bool bWonGame)
{
	if (IsLocallyControlled())
	{
		// Disable Input
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		this->DisableInput(PlayerController);
		
		// Show Mouse Cursor
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}

		HandleDestruction();

		// Spawn Menu Widget
		UGameOverWidget* GameOverWidget =CreateWidget<UGameOverWidget>(GetWorld(),
		GameOverWidgetClass);

		if (GameOverWidget)
		{
			GameOverWidget->SetText(bWonGame);
			GameOverWidget->AddToViewport();
		}

	}
}


void ABasePlayer::HandleDestruction()
{
	Destroy();
}


void ABasePlayer::OnTakeDamageMontageCompleted(UAnimMontage* AnimMontage, bool bInterrupted)
{

	UE_LOG(LogTemp, Warning, TEXT("ABasePlayer::OnTakeDamageCompleted"));
	
	
	if (AnimMontage == TakeDamageMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasePlayer::OnTakeDamageCompleted"));
		
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (PlayerController)
		{
			this->EnableInput(PlayerController);
		}
	}
}





#pragma endregion