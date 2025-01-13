// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HealthComponent.h"
// #include "Interfaces/OnlineSessionInterface.h"
#include "BaseUserWidget.h"

#include "BasePlayer.generated.h"


// Declare a multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustomTakeDamage, int, Health);

UCLASS()
class LASERTENNISONLINE_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;

	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



//
// Components
//
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement) 
	class UCustomCharacterMovementComponent* CustomCharacterMovementComponent;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) 
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) 
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true")) 
	class UHealthComponent* HealthComponent;

//
// Constructor
//
public:
	ABasePlayer(const FObjectInitializer& ObjectInitializer);


//
// Inputs
// 
private:
	// Input Map Context
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapContext;
	// Input Map Context
	
	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* moveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* jumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* pauseGameAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* dodgeAction;
	
public:
	// Input Binding Functions
	void move(const FInputActionValue& value);
	void jump(const FInputActionValue& value);
	void pauseGame(const FInputActionValue& value);
	void dodge(const FInputActionValue& value);


//
// Gameplay
//
public:

	UFUNCTION(NetMulticast, Reliable)
	void GamePreStart();

	UFUNCTION(NetMulticast, Reliable)
	void GameStart();

	UFUNCTION(NetMulticast, Reliable)
	void CustomTakeDamage();

	UFUNCTION(NetMulticast, Reliable)
	void GameOver(bool bWonGame, APawn* DefaultPawn);

	UPROPERTY(EditDefaultsOnly) UAnimMontage* TakeDamageMontage; // (Animation)
	
	UPROPERTY(EditDefaultsOnly) UAnimMontage* DoubleJumpMontage; // (Animation)


	bool bIsAlive() const {return HealthComponent->bIsPlayerAlive();};
	int GetPlayerHealth() const {return HealthComponent->GetPlayerHealth();};	

	UFUNCTION()
	void OnTakeDamageMontageCompleted(UAnimMontage* AnimMontage, bool bInterrupted);

	void HandleDestruction(APawn* DefaultPawn);

	FOnCustomTakeDamage OnCustomTakeDamage;


//
// UI
//

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> GameInputsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> GameTutorialWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> GameStartCountdownClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> GameOverVictoryClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> GameOverDefeatClass;

	UBaseUserWidget* GameInputsWidget;
	UBaseUserWidget* GameTutorialWidget;
	UBaseUserWidget* GameStartCountdown;


public:
	void DisplayLobbyWidgets();
	void DisplayCountdown();

	void RemoveLobbyWidgets();



	// IOnlineSessionPtr OnlineSessionInterface2;

};
