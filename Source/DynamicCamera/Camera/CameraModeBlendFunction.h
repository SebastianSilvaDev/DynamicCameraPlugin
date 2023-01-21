// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CameraModeBlendFunction.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DYNAMICCAMERA_API UCameraModeBlendFunction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetBlendFunctionResult(float Weight, float Exponent);
	
	virtual float GeInverseExponent(float BlendExponent);
};
