// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCameraMode.h"

#include "Curves/CurveVector.h"

void UThirdPersonCameraMode::UpdateView(float DeltaTime)
{
	Super::UpdateView(DeltaTime);
	
	if (!IsValid(TargetOffsetCurve)) return;
	const FVector OffsetVector = TargetOffsetCurve->GetVectorValue(View.Rotation.Pitch);
#if WITH_EDITOR
	if (bEnableDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("%f ===> %s"), View.Rotation.Pitch, *OffsetVector.ToString()));
	}
#endif
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
#if WITH_EDITOR
	if (bEnableDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("%s ===> %f"), *View.Location.ToString(), Hit.Distance));
	}
#endif
	View.Location += Location;
}
