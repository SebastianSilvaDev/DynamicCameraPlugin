// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraModeBlendFunction.h"


float UCameraModeBlendFunction::GetBlendFunctionResult_Implementation(float Weight, float Exponent)
{
	return Weight;
}

float UCameraModeBlendFunction::GeInverseExponent(const float BlendExponent)
{
	return (BlendExponent > 0.f) ? (1.0f / BlendExponent) : 1.f;
}
