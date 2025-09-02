// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CeilingLight.generated.h"

UCLASS()
class LASERTENNISONLINE_API ACeilingLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACeilingLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* Light;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 
	// Lighting
	//
	void ChangeLightColor(FLinearColor NewColor);

private:

	FLinearColor DefaultMeshColor;
	FLinearColor DefaultLightColor;
	bool Ready = true;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	void ReturnToDefault();
};
