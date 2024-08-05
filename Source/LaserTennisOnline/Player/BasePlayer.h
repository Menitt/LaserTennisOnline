// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "BasePlayer.generated.h"


UCLASS()
class LASERTENNISONLINE_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UPROPERTY(EditDefaultsOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* inputMapContext;
	
	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* moveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* jumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* pauseGameAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* dodgeAction;
	

	// Input Binding Functions
	void move(const FInputActionValue& value);
	void jump(const FInputActionValue& value);
	void pauseGame(const FInputActionValue& value);

//
// Gameplay
//
public:
	UFUNCTION(Client, Reliable)
	void CustomTakeDamage();

	UFUNCTION(Client, Reliable)
	void GameOver(bool bWonGame);





};
