// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCameraMode.h"

#include "Curves/CurveVector.h"

void UThirdPersonCameraMode::UpdateView(float DeltaTime)
{
	Super::UpdateView(DeltaTime);
	
	if (!IsValid(TargetOffsetCurve)) return;
	const FVector OffsetVector = TargetOffsetCurve->GetVectorValue(View.Rotation.Pitch);
	View.Location = View.Location + View.Rotation.RotateVector(OffsetVector);
	PreventCollision(DeltaTime);
}

void UThirdPersonCameraMode::PreventCollision(float DeltaTime)
{
	FHitResult Hit;
	FCollisionQueryParams QueryParams(FName("Camera"), false, GetTargetActor());
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(0.f);
	const bool bHasHit = GetWorld()->SweepSingleByChannel(Hit, GetTargetActor()->GetActorLocation(), View.Location, FRotator::ZeroRotator.Quaternion(), ECC_Camera, SphereShape, QueryParams);
	if (!bHasHit) return;
	FVector Location = Hit.Location - View.Location;
	View.Location = View.Location + Location;
}
