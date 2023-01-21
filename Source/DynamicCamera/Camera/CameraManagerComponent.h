// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManagerComponent.generated.h"


class UCameraModeBase;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DYNAMICCAMERA_API UCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraManagerComponent();

	UFUNCTION(BlueprintCallable)
	void SetCameraMode(TSubclassOf<UCameraModeBase> NewCameraModeClass);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	TSubclassOf<UCameraModeBase> GetLatestCameraMode() const;

	UPROPERTY()
	TSubclassOf<UCameraModeBase> LatestCameraMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraModeBase> DefaultCameraMode;
};
