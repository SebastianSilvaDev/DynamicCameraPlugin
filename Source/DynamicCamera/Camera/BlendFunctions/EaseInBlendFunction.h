// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicCamera/Camera/CameraModeBlendFunction.h"
#include "UObject/Object.h"
#include "EaseInBlendFunction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DYNAMICCAMERA_API UEaseInBlendFunction : public UCameraModeBlendFunction
{
	GENERATED_BODY()

public:
	virtual float GetBlendFunctionResult_Implementation(float Weight, float Exponent) override;
};
