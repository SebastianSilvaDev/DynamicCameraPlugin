// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicCameraComponent.h"
#include "CameraModeBase.h"
#include "CameraStackManager.h"


// Sets default values for this component's properties
UDynamicCameraComponent::UDynamicCameraComponent()
{
}

void UDynamicCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	if (!IsValid(StackManager)) return;

	check(StackManager);

	UpdateCameraModes();
	
	FCameraModeView ModeView;
	StackManager->EvaluateStack(DeltaTime, ModeView);
	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()); TargetPawn && IsValid(TargetPawn->GetController()))
	{
		TargetPawn->GetController()->SetControlRotation(ModeView.ControlRotation);
	}

	SetWorldLocationAndRotation(ModeView.Location, ModeView.Rotation);
	FieldOfView = ModeView.FieldOfView;

	DesiredView.Location = ModeView.Location;
	DesiredView.Rotation = ModeView.Rotation;
	DesiredView.FOV = ModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UDynamicCameraComponent::OnRegister()
{
	Super::OnRegister();
	
	if (!IsValid(StackManager))
	{
		StackManager = NewObject<UCameraStackManager>(this);
	}
}


// Called when the game starts
void UDynamicCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!ensureMsgf(IsValid(StackManager), TEXT("The stack manager hasn't been initialized!"))) return;
}

void UDynamicCameraComponent::UpdateCameraModes()
{
	check(StackManager);

	if (!StackManager->IsStackActivate()) return;
	if (!DetermineCameraModeDelegate.IsBound()) return;
	if (const TSubclassOf<UCameraModeBase> CameraMode = DetermineCameraModeDelegate.Execute())
	{
		StackManager->PushCameraMode(CameraMode);
	}
}
