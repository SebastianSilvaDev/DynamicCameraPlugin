// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicCamera/Camera/CameraModeBase.h"
#include "UObject/Object.h"
#include "ThirdPersonCameraMode.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DYNAMICCAMERA_API UThirdPersonCameraMode : public UCameraModeBase
{
	GENERATED_BODY()

protected:
	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="OffsetCurve")
	const UCurveVector* TargetOffsetCurve;
};
