// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceAbilitiesModule, ChaliceAbilities);
DEFINE_LOG_CATEGORY(LogChaliceAbilities);

void FChaliceAbilitiesModule::StartupModule()
{
	UE_LOG(LogChaliceAbilities, Display, TEXT("ChaliceAbilities: Module started"));
}

void FChaliceAbilitiesModule::ShutdownModule()
{
	UE_LOG(LogChaliceAbilities, Display, TEXT("ChaliceAbilities: Module shutdown"));
}
