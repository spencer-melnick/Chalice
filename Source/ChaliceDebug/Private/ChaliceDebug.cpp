// Copyright (c) 2020 Spencer Melnick

#include "ChaliceDebug.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceDebugModule, ChaliceDebug);
DEFINE_LOG_CATEGORY(LogChaliceDebug);

void FChaliceDebugModule::StartupModule()
{
	UE_LOG(LogChaliceDebug, Display, TEXT("ChaliceDebug: Module started"));
}

void FChaliceDebugModule::ShutdownModule()
{
	UE_LOG(LogChaliceDebug, Display, TEXT("ChaliceDebug: Module shutdown"));
}
