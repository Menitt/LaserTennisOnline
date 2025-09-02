// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"



// Sets default values
ACeilingLight::ACeilingLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	// Components 
	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Light = CreateDefaultSubobject<USpotLightComponent>("Light");
	RootComponent = Mesh;
	Light->SetupAttachment(Mesh);
}

void ACeilingLight::ChangeLightColor(FLinearColor NewColor)
{
	if (not Ready)
	{
		return;
	}
	else
	{
		Ready = false;
		// Light
		if(IsValid(Light))
		{
			Light->SetLightColor(NewColor);
		}
		// Light Mesh Material
		if (IsValid(DynamicMaterial))
		{
			DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), NewColor);
		}
	
		float Duration = FMath::FRandRange(0.5f, 2.5f);
		FTimerHandle DummyHandle;
		GetWorld()->GetTimerManager().SetTimer(DummyHandle, this, &ThisClass::ReturnToDefault, Duration, false);
	}
	

}

void ACeilingLight::ReturnToDefault()
{
	
	if (IsValid(Light))
	{
		Light->SetLightColor(DefaultLightColor);
	}
	if (IsValid(DynamicMaterial))
	{
		DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), DefaultMeshColor);
	}

	Ready = true;
}



// Called when the game starts or when spawned
void ACeilingLight::BeginPlay()
{
	Super::BeginPlay();
	
	// 
	// Set Mesh Material to Dynamic and read Default Colors
	//
	UMaterialInterface* Material = Mesh->GetMaterial(1);
	if (IsValid(Material))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		Mesh->SetMaterial(1, DynamicMaterial);
		if (IsValid(DynamicMaterial))
		{
			DynamicMaterial->GetVectorParameterValue(FMaterialParameterInfo("EmissiveColor"), DefaultMeshColor);
		}
		
	}

	if (IsValid(Light))
	{
		DefaultLightColor = Light->GetLightColor();
	}
}




// Called every frame
void ACeilingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

