// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MegaScreen.generated.h"

UCLASS()
class LASERTENNISONLINE_API AMegaScreen : public AActor
{
	GENERATED_BODY()
	

// 
// COMPONENTS
// 
private:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* CountdownWidgetComp;	

public:	
	// Sets default values for this actor's properties
	AMegaScreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 
// GAMEPLAY
// 
public:
	void StartCountdown(int Timer);

};
