// Fill out your copyright notice in the Description page of Project Settings.


#include "EaseInBlendFunction.h"

float UEaseInBlendFunction::GetBlendFunctionResult_Implementation(float Weight, float Exponent)
{
	return FMath::InterpEaseIn(0.f, 1.f, Weight, Exponent);
}
