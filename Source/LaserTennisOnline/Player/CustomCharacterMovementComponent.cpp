// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include "BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


#pragma region Saved Move

UCustomCharacterMovementComponent::FSavedMove_LaserTennisOnline::FSavedMove_LaserTennisOnline(){
    Saved_bWantsToSprint=0;
}

// Checks if two Moves are "equivalent" and can be combined, for perfomance and efficiency reasons.
bool UCustomCharacterMovementComponent::FSavedMove_LaserTennisOnline::CanCombineWith(const FSavedMovePtr &NewMove, 
ACharacter *InCharacter, float MaxDelta) const
{
    const FSavedMove_LaserTennisOnline* newLaserMove = static_cast<FSavedMove_LaserTennisOnline*>(NewMove.Get());

    if (not newLaserMove) return false;

    if (Saved_bWantsToSprint != newLaserMove->Saved_bWantsToSprint)
    {
        return false;
    }

    if (Saved_bWantsToDash != newLaserMove->Saved_bWantsToDash)
	{
		return false;
	}
    
    return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UCustomCharacterMovementComponent::FSavedMove_LaserTennisOnline::Clear()
{
    FSavedMove_Character::Clear();
    Saved_bWantsToDash = 0;
}

uint8 UCustomCharacterMovementComponent::FSavedMove_LaserTennisOnline::GetCompressedFlags() const
{
    uint8 Result = FSavedMove_Character::GetCompressedFlags();
    
    if (Saved_bWantsToDash) Result |= FLAG_Dash;

    return Result;
}

// This function captures the movement data-> we want to use this function to store and update our wokring variables in the
// FSaved move class: setting the current move for the move-snapshot class.
void UCustomCharacterMovementComponent::FSavedMove_LaserTennisOnline::SetMoveFor(
ACharacter* C, float InDeltaTime, FVector const& NewAccel, 
FNetworkPredictionData_Client_Character& ClientData){
    
    FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

    const UCustomCharacterMovementComponent* CharacterMovement = Cast<UCustomCharacterMovementComponent>(C->GetCharacterMovement());

    Saved_bWantsToDash = CharacterMovement->Safe_bWantsToDash;

}


// This function reads the information from Saved Move 
void UCustomCharacterMovementComponent::FSavedMove_LaserTennisOnline::PrepMoveFor(ACharacter *C)
{
    FSavedMove_Character::PrepMoveFor(C);

    UCustomCharacterMovementComponent* characterMovement = 
    Cast<UCustomCharacterMovementComponent>(C->GetCharacterMovement());
    
    characterMovement->Safe_bWantsToDash = Saved_bWantsToDash;

}

#pragma endregion

#pragma region Client Network Predicition Data

UCustomCharacterMovementComponent::FNetworkPredictionData_Client_LaserTennisOnline::FNetworkPredictionData_Client_LaserTennisOnline(
    const UCharacterMovementComponent &ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UCustomCharacterMovementComponent::FNetworkPredictionData_Client_LaserTennisOnline::AllocateNewMove()
{
    return FSavedMovePtr(new FSavedMove_LaserTennisOnline());
}

#pragma endregion


#pragma region CMC

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(){
}

void UCustomCharacterMovementComponent::InitializeComponent()
{
    Super::InitializeComponent();
    PlayerOwner = Cast<ABasePlayer>(GetOwner());
}

// Network
FNetworkPredictionData_Client *UCustomCharacterMovementComponent::GetPredictionData_Client() const
{
    check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UCustomCharacterMovementComponent* MutableThis = const_cast<UCustomCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_LaserTennisOnline(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f; 
	}
	return ClientPredictionData;
}

void UCustomCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
    Super::UpdateFromCompressedFlags(Flags);

    Safe_bWantsToDash = (Flags & FSavedMove_LaserTennisOnline::FLAG_Dash) != 0;
}

// Movement Pipeline
void UCustomCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
    // Dash
    bool bAuthProxy = CharacterOwner->HasAuthority() && !CharacterOwner->IsLocallyControlled();
	if (Safe_bWantsToDash && CanDash())
	{
		if (!bAuthProxy || GetWorld()->GetTimeSeconds() - DashStartTime > AuthDashCooldownDuration)
		{
			PerformDash();
			Safe_bWantsToDash = false;
			Proxy_bDash = !Proxy_bDash;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Client tried to cheat"))
		}
	}

    Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector &OldLocation, const FVector &OldVelocity)
{
    Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

// Movement Event
bool UCustomCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
    return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}


void UCustomCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (PreviousMovementMode == MOVE_Falling && MovementMode == MOVE_Walking)
    {
        // Character just landed on the ground
        if (ABasePlayer* OwnerPlayer = Cast<ABasePlayer>(GetOwner()))
        {
            // Play your land sound
            OwnerPlayer->PlayLandSound();
        }
    }

}

#pragma endregion

#pragma region Dash

void UCustomCharacterMovementComponent::OnDashCooldownFinished()
{
	Safe_bWantsToDash = true;
}

bool UCustomCharacterMovementComponent::CanDash() const
{
	return IsWalking() && !IsCrouching() || IsFalling();
}

void UCustomCharacterMovementComponent::PerformDash()
{
	DashStartTime = GetWorld()->GetTimeSeconds();

    float DashImpulse = (MovementMode == MOVE_Falling) ? DashImpulseAir : DashImpulseLand; 

	FVector DashDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration).GetSafeNormal2D();
    DashDirection += FVector::UpVector * DashUpwardScaler;
    Velocity = DashImpulse * DashDirection;

    FQuat NewRotation = FRotationMatrix::MakeFromXZ(DashDirection, FVector::UpVector).ToQuat();
    FHitResult Hit;

    SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
    
    SetMovementMode(MOVE_Falling);
	
	CharacterOwner->PlayAnimMontage(DashMontage);
	DashStartDelegate.Broadcast();
}

#pragma endregion


#pragma region Interface

void UCustomCharacterMovementComponent::DashPressed()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - DashStartTime >= DashCooldownDuration)
	{
		Safe_bWantsToDash = true;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DashCooldown, this, &UCustomCharacterMovementComponent::OnDashCooldownFinished, DashCooldownDuration - (CurrentTime - DashStartTime));
	}
}

void UCustomCharacterMovementComponent::DashReleased()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DashCooldown);
	Safe_bWantsToDash = false;
}

#pragma endregion

#pragma region Replication

void UCustomCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCustomCharacterMovementComponent, Proxy_bDash, COND_SkipOwner);
}


void UCustomCharacterMovementComponent::OnRep_Dash()
{
	// CharacterOwner->PlayAnimMontage(DashMontage);
    DashStartDelegate.Broadcast();
}

#pragma endregion