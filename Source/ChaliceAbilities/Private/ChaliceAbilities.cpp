// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities.h"
#include "ChaliceAbilities/System/GameplayDebuggerCategory_ChaliceAbilities.h"
#include "GameplayDebugger.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceAbilitiesModule, ChaliceAbilities);
DEFINE_LOG_CATEGORY(LogChaliceAbilities);

void FChaliceAbilitiesModule::StartupModule()
{
	UE_LOG(LogChaliceAbilities, Display, TEXT("ChaliceAbilities: Module started"));

#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("ChaliceAbilities", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_ChaliceAbilities::MakeInstance));
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FChaliceAbilitiesModule::ShutdownModule()
{
	UE_LOG(LogChaliceAbilities, Display, TEXT("ChaliceAbilities: Module shutdown"));

#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("ChaliceAbilities");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif
}
