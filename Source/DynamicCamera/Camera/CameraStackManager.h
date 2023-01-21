// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CameraStackManager.generated.h"

class UCameraModeBase;
struct FCameraModeView;

/**
 * 
 */
UCLASS()
class DYNAMICCAMERA_API UCameraStackManager : public UObject
{
	GENERATED_BODY()

public:
	UCameraStackManager();

	UFUNCTION()
	virtual void ActivateStack();

	UFUNCTION()
	virtual void DeactivateStack();

	bool IsStackActivate() const
	{
		return bIsActive;
	}

	void PushCameraMode(TSubclassOf<UCameraModeBase> CameraModeClass);

	bool EvaluateStack(float DeltaTime, OUT FCameraModeView& OutCameraModeView);

	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTagContainer& OutTypeOfCameraMode) const;
	
protected:

	UFUNCTION(BlueprintCallable)
	UCameraModeBase* GetCameraModeInstance(TSubclassOf<UCameraModeBase> CameraModeClass);

	UFUNCTION(BlueprintCallable)
	UCameraModeBase* GetCameraModeInstanceByTypes(FGameplayTagContainer TagContainer);

	void UpdateStack(float DeltaTime);

	void BlendStack(FCameraModeView& CameraModeView) const;
	
	bool bIsActive;

	UPROPERTY(Instanced, EditDefaultsOnly)
	TArray<UCameraModeBase*> CameraModeInstances;

	UPROPERTY()
	TArray<UCameraModeBase*> CameraModeStack;
};
