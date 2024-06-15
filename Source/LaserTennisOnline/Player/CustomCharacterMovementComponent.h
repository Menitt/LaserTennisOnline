// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"


// Custom Delegate for player Dashing (no inputs)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

// Custom Movement Mode Enum:
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None	UMETA(Hidden),
	CMOVE_Slide UMETA(DisplayName = "Slide"),
	CMOVE_MAX	UMETA(Hidden),
};


UCLASS()
class LASERTENNISONLINE_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	//
	// Saved Move Class
	//
	// This class represents a snap-shot of all the state date information required to produce
	// a move -> every frame the information in this class is compressed and sent to the server.
	class FSavedMove_LaserTennisOnline : public FSavedMove_Character
	{
		public:
		enum CompressedFlags
		{
			FLAG_Sprint			= 0x10,
			FLAG_Dash			= 0x20,
			FLAG_Custom_2		= 0x40,
			FLAG_Custom_3		= 0x80,
		};


		// Flags
		uint8 Saved_bPressedZippyJump:1;
		uint8 Saved_bWantsToSprint:1;
		uint8 Saved_bWantsToDash:1;

		// Other Variables
		uint8 Saved_bHadAnimRootMotion:1;
		uint8 Saved_bTransitionFinished:1;
		uint8 Saved_bWallRunIsRight:1;
		
		FSavedMove_LaserTennisOnline();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	//
	// Network Prediction Data Client Class.
	//
	// The purpose of this class is to tell the character movement component to use our custom Saved Move 
	// class instead of the parent one.
	class FNetworkPredictionData_Client_LaserTennisOnline: public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_LaserTennisOnline(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};


//
// Parameters
//
private:

	// Dash
	UPROPERTY(EditDefaultsOnly) float DashImpulseLand=1200.f;
	UPROPERTY(EditDefaultsOnly) float DashImpulseAir=900.f;
	UPROPERTY(EditDefaultsOnly) float DashCooldownDuration=1.f;
	UPROPERTY(EditDefaultsOnly) float DashUpwardScaler=0.3f;
	UPROPERTY(EditDefaultsOnly) float AuthDashCooldownDuration=.9f; // (Anti-cheat)
	// UPROPERTY(EditDefaultsOnly) UAnimMontage* DashMontage; // (Animation)

//
// Transient
//
private:
	UPROPERTY(Transient) class ABasePlayer* PlayerOwner;
	// FLAGS
	// working variable whose value is regularly copied inot the FSavedMove object.
	bool Safe_bWantsToDash;
	float DashStartTime;
	FTimerHandle TimerHandle_DashCooldown;

//
// Replication & Delegate
//
private:
	UPROPERTY(ReplicatedUsing=OnRep_Dash) bool Proxy_bDash;
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;
	
	UFUNCTION() void OnRep_Dash();
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

//
// Constructor & Initialisation
//
public:
	UCustomCharacterMovementComponent();
protected:
	virtual void InitializeComponent() override;


//
// Overriding Character Movement Comp
//
public:
	// This function is used to tell the character movement to use out custom FNetworkClient class
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, 
	const FVector& OldVelocity) override;

//
// Custom Movement Implementation
//
private:
	// Dash
	void OnDashCooldownFinished();
	bool CanDash() const;
	void PerformDash();

//
// Custom Movement Interface
//
public:
	UFUNCTION(BlueprintCallable) void DashPressed();
	UFUNCTION(BlueprintCallable) void DashReleased();

//
// Helper Functions
//
public:
	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;


};
