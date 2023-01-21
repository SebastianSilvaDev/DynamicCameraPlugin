#include "DynamicCamera.h"


#define LOCTEXT_NAMESPACE "FDynamicCamera"

void FDynamicCamera::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FDynamicCamera::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDynamicCamera, DynamicCamera)