// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBeltObject.generated.h"

UCLASS()
class LASERTENNISONLINE_API AMovingBeltObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingBeltObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//
// Components
//
private:

	UPROPERTY(EditDefaultsOnly)
	float Speed = 400;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComponent;

};
