// Copyright (c) 2020 Spencer Melnick

#include "ChaliceEditor.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FChaliceEditorModule, ChaliceEditor);
DEFINE_LOG_CATEGORY(LogChaliceEditor);

void FChaliceEditorModule::StartupModule()
{
	UE_LOG(LogChaliceEditor, Display, TEXT("ChaliceEditor: Module started"));
}

void FChaliceEditorModule::ShutdownModule()
{
	UE_LOG(LogChaliceEditor, Display, TEXT("ChaliceEditor: Module shutdown"));
}
