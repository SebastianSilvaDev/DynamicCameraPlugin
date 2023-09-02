// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraModeBase.h"

#include "DynamicCameraComponent.h"
#include "BlendFunctions/LinearBlendFunction.h"

FCameraModeView::FCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(80.f)
{
}

void FCameraModeView::Blend(const FCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UCameraModeBase::UCameraModeBase()
{
	FieldOfView = 80.f;
	ViewPitchMin = -89.9f;
	ViewPitchMax = 89.9f;

	BlendTime = 0.5f;
	CameraModeBlendFunction = ULinearBlendFunction::StaticClass();
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
}

UWorld* UCameraModeBase::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

AActor* UCameraModeBase::GetTargetActor() const
{
	const UDynamicCameraComponent* DynamicCameraComponent = CastChecked<UDynamicCameraComponent>(GetOuter());
	return DynamicCameraComponent->GetOwner();
}

UDynamicCameraComponent* UCameraModeBase::GetOuterAsDynamicCamera() const
{
	return static_cast<UDynamicCameraComponent*>(GetOuter());
}

void UCameraModeBase::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UCameraModeBase::SetBlendWeight(float Weight)
{
	if(!ensure(IsValid(CameraModeBlendFunction))) return;
	BlendWeight = FMath::Clamp(Weight, 0.f, 1.f);

	const float InvExponent = CameraModeBlendFunction.GetDefaultObject()->GeInverseExponent(BlendExponent);
	
	BlendAlpha = CameraModeBlendFunction.GetDefaultObject()->GetBlendFunctionResult(Weight, InvExponent);
}

FVector UCameraModeBase::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UCameraModeBase::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetControlRotation();
	}

	return TargetActor->GetActorRotation();
}

void UCameraModeBase::GetPivotViewLocationAndRotation(FVector& PivotLocation, FRotator& PivotRotation)
{
	const AActor* TargetActor = GetTargetActor();
	TargetActor->GetActorEyesViewPoint(PivotLocation, PivotRotation);
}

void UCameraModeBase::UpdateView(float DeltaTime)
{
	FVector PivotLocation;
	FRotator PivotRotation;
	GetPivotViewLocationAndRotation(PivotLocation, PivotRotation);

	PivotRotation.Pitch = FMath::ClampAngle<float>(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	
}

void UCameraModeBase::UpdateBlending(float DeltaTime)
{
	if (!ensure(IsValid(CameraModeBlendFunction))) return;
	if (BlendTime > 0.f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.f);
	}
	else
	{
		BlendAlpha = 1.f;
	}

	const float Exponent = (BlendExponent > 0.f) ? BlendExponent : 1.0f;
	BlendWeight = CameraModeBlendFunction.GetDefaultObject()->GetBlendFunctionResult(BlendAlpha, Exponent);
	
}
