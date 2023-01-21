// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManagerComponent.h"

#include "CameraModeBase.h"
#include "DynamicCameraComponent.h"


// Sets default values for this component's properties
UCameraManagerComponent::UCameraManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCameraManagerComponent::SetCameraMode(TSubclassOf<UCameraModeBase> NewCameraModeClass)
{
	if (!ensure(IsValid(NewCameraModeClass))) return;
	LatestCameraMode = NewCameraModeClass;
}


// Called when the game starts
void UCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	UDynamicCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UDynamicCameraComponent>();
	if (IsValid(CameraComponent))
	{
		CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::GetLatestCameraMode);
	}
}

TSubclassOf<UCameraModeBase> UCameraManagerComponent::GetLatestCameraMode() const
{
	if (IsValid(LatestCameraMode))
	{
		return LatestCameraMode;	
	}
	if (!ensure(IsValid(DefaultCameraMode))) return nullptr;
	return DefaultCameraMode;
}
