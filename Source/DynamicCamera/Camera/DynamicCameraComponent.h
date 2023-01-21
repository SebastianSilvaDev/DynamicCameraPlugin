// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "DynamicCameraComponent.generated.h"

class UCameraStackManager;
class UCameraModeBase;

DECLARE_DELEGATE_RetVal(TSubclassOf<UCameraModeBase>, FCameraModeDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DYNAMICCAMERA_API UDynamicCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDynamicCameraComponent();

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void OnRegister() override;

	virtual AActor* GetTargetActor() const { return GetOwner(); }

	FCameraModeDelegate DetermineCameraModeDelegate;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void UpdateCameraModes();

	UPROPERTY()
	TObjectPtr<UCameraStackManager> StackManager = nullptr;
};
