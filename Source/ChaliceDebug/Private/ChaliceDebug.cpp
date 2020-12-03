// Copyright (c) 2020 Spencer Melnick

#include "ChaliceDebug.h"
#include "ChaliceDebug/WeaponTraceDebug.h"
#include "GameFramework/HUD.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceDebugModule, ChaliceDebug);
DEFINE_LOG_CATEGORY(LogChaliceDebug);

void FChaliceDebugModule::StartupModule()
{
	UE_LOG(LogChaliceDebug, Display, TEXT("ChaliceDebug: Module started"));

	DisplayDelegateHandles.Add(AHUD::OnShowDebugInfo.AddStatic(&FWeaponTraceDebug::OnShowDebugInfo));
}

void FChaliceDebugModule::ShutdownModule()
{
	UE_LOG(LogChaliceDebug, Display, TEXT("ChaliceDebug: Module shutdown"));

	for (FDelegateHandle& DelegateHandle : DisplayDelegateHandles)
	{
		AHUD::OnShowDebugInfo.Remove(DelegateHandle);
	}
}
