// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCameraMode.h"

#include "Curves/CurveVector.h"

void UThirdPersonCameraMode::UpdateView(float DeltaTime)
{
	Super::UpdateView(DeltaTime);
	
	if (!IsValid(TargetOffsetCurve)) return;
	const FVector OffsetVector = TargetOffsetCurve->GetVectorValue(View.Rotation.Pitch);
	View.Location = View.Location + View.Rotation.RotateVector(OffsetVector);
}
