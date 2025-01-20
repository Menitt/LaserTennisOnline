// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPanel.generated.h"

UCLASS()
class LASERTENNISONLINE_API AHealthPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* HealthWidget;

	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	bool bShouldMove = false;
	float TimeToPosition = 3.f;

	float DistanceToLocation = 100;


public:
	UFUNCTION(NetMulticast, Reliable)
	void UpdateWidgetHealth(int PlayerHealth);

	UFUNCTION(NetMulticast, Reliable)
	void InitializeWidgetText(const FString& InitialText, int PlayerHealth);

	UFUNCTION(NetMulticast, Reliable)
	void Activate(float Period);


};
