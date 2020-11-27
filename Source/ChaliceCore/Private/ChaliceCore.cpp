// Copyright (c) 2020 Spencer Melnick

#include "ChaliceCore.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceCoreModule, ChaliceCore);
DEFINE_LOG_CATEGORY(LogChaliceCore);

void FChaliceCoreModule::StartupModule()
{
	UE_LOG(LogChaliceCore, Display, TEXT("ChaliceCore: Module started"));
}

void FChaliceCoreModule::ShutdownModule()
{
	UE_LOG(LogChaliceCore, Display, TEXT("ChaliceCore: Module shutdown"));
}
