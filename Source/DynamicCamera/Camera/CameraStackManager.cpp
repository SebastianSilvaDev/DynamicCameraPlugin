// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraStackManager.h"

#include "CameraModeBase.h"

UCameraStackManager::UCameraStackManager()
{
	bIsActive = true;
}

void UCameraStackManager::ActivateStack()
{
	if (bIsActive) return;
	bIsActive = true;

	for (UCameraModeBase* CameraMode : CameraModeStack)
	{
		CameraMode->OnActivation();
	}
}

void UCameraStackManager::DeactivateStack()
{
	if (!bIsActive) return;
	bIsActive = false;

	for (UCameraModeBase* CameraMode : CameraModeStack)
	{
		CameraMode->OnDeactivation();
	}
}

void UCameraStackManager::PushCameraMode(TSubclassOf<UCameraModeBase> CameraModeClass)
{
	if (!IsValid(CameraModeClass))
	{
		return;
	}

	UCameraModeBase* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();

	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->GetBlendWeight();
			break;
		}

		ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->GetBlendWeight());
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->GetBlendTime() > 0.0f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ?ExistingStackContribution : 1.0f);

	CameraMode->SetBlendWeight(BlendWeight);

	CameraModeStack.Insert(CameraMode, 0);

	CameraModeStack.Last()->SetBlendWeight(1.0f);

	if (ExistingStackIndex == INDEX_NONE)
	{
		CameraMode->OnActivation();
	}
}

bool UCameraStackManager::EvaluateStack(float DeltaTime, FCameraModeView& OutCameraModeView)
{
	if (!bIsActive)
	{
		return false;
	}
	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);
	return true;
}

void UCameraStackManager::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTagContainer& OutTypeOfCameraMode) const
{
	if (CameraModeStack.Num() == 0)
	{
		OutWeightOfTopLayer = 1.f;
		OutTypeOfCameraMode = FGameplayTagContainer();
		return;
	}

	UCameraModeBase* TopEntry = CameraModeStack.Last();
	check(TopEntry);
	OutWeightOfTopLayer = TopEntry->GetBlendWeight();
	OutTypeOfCameraMode = TopEntry->GetCameraModeTypes();
}

UCameraModeBase* UCameraStackManager::GetCameraModeInstance(TSubclassOf<UCameraModeBase> CameraModeClass)
{
	check(CameraModeClass);

	for (UCameraModeBase* CameraMode : CameraModeInstances)
	{
		if (IsValid(CameraMode) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	UCameraModeBase* NewCameraMode = NewObject<UCameraModeBase>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

UCameraModeBase* UCameraStackManager::GetCameraModeInstanceByTypes(FGameplayTagContainer TagContainer)
{
	for (UCameraModeBase* CameraMode : CameraModeInstances)
	{
		if (IsValid(CameraMode) && (CameraMode->GetCameraModeTypes().HasAll(TagContainer)))
		{
			return CameraMode;
		}
	}
	return nullptr;
}

void UCameraStackManager::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UCameraModeBase* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if (CameraMode->GetBlendWeight() >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount <= 0) return;
	
	for (int32 StackIndex = RemoveIndex; StackIndex < StackSize; ++StackIndex)
	{
		UCameraModeBase* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->OnDeactivation();
	}
	CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
}

void UCameraStackManager::BlendStack(FCameraModeView& CameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	const UCameraModeBase* CameraMode = CameraModeStack[StackSize-1];
	check(CameraMode);

	CameraModeView = CameraMode->GetCameraModeView();

	for (int32 StackIndex = (StackSize - 2); StackIndex >=0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
	}
}
