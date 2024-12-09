// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStartPanel.generated.h"

UCLASS()
class LASERTENNISONLINE_API AGameStartPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameStartPanel();

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
	class UWidgetComponent* GameStartWidget;


public:

	void StartCountdown();



};
 