// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CameraModeBase.generated.h"

USTRUCT(BlueprintType)
struct FCameraModeView
{
	GENERATED_BODY()

	FCameraModeView();

	void Blend(const FCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

class UCameraModeBlendFunction;

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, DisplayName="CameraMode")
class DYNAMICCAMERA_API UCameraModeBase : public UObject
{
	GENERATED_BODY()

public:
	UCameraModeBase();

	virtual UWorld* GetWorld() const override;

	virtual AActor* GetTargetActor() const;

	UFUNCTION(BlueprintGetter)
	const FCameraModeView& GetCameraModeView() const { return View; }

	virtual void OnActivation()
	{
	};

	virtual void OnDeactivation()
	{
	};

	virtual void UpdateCameraMode(float DeltaTime);

	UFUNCTION(BlueprintGetter)
	float GetBlendTime() const
	{
		return BlendTime;
	}

	UFUNCTION(BlueprintGetter)
	float GetBlendWeight() const { return BlendWeight; }

	UFUNCTION(BlueprintSetter)
	virtual void SetBlendWeight(float Weight);

	UFUNCTION(BlueprintGetter)
	FGameplayTagContainer GetCameraModeTypes() const
	{
		return CameraModeTypes;
	}

protected:

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;
	virtual void GetPivotViewLocationAndRotation(FVector& PivotLocation, FRotator& PivotRotation);

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);
	
	UPROPERTY(EditDefaultsOnly, Category="Types", BlueprintGetter=GetCameraModeTypes)
	FGameplayTagContainer CameraModeTypes;

	UPROPERTY(BlueprintGetter=GetCameraModeView)
	FCameraModeView View;

	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="5.0", UIMax="170", ClampMin="5.0", ClampMax="170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category="Blending", BlueprintGetter=GetBlendTime)
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	TSubclassOf<UCameraModeBlendFunction> CameraModeBlendFunction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendExponent;

	UPROPERTY(BlueprintGetter=GetBlendWeight, BlueprintSetter=SetBlendWeight)
	float BlendWeight;

	UPROPERTY()
	float BlendAlpha;

	UPROPERTY(Transient)
	uint32 bResetInterpolation:1;
};
