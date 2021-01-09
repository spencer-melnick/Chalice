// Copyright (c) 2020 Spencer Melnick

#include "ChaliceUI.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceUIModule, ChaliceUI);
DEFINE_LOG_CATEGORY(LogChaliceUI);

void FChaliceUIModule::StartupModule()
{
	UE_LOG(LogChaliceUI, Display, TEXT("ChaliceUI: Module started"));
}

void FChaliceUIModule::ShutdownModule()
{
	UE_LOG(LogChaliceUI, Display, TEXT("ChaliceUI: Module shutdown"));
}
