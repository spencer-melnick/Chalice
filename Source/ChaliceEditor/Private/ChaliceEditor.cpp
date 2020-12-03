// Copyright (c) 2020 Spencer Melnick

#include "ChaliceEditor.h"
#include "ComponentVisualizers/WeaponTraceVisualizer.h"
#include "ChaliceGame/Combat/WeaponTraceComponent.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

IMPLEMENT_GAME_MODULE(FChaliceEditorModule, ChaliceEditor);
DEFINE_LOG_CATEGORY(LogChaliceEditor);

void FChaliceEditorModule::StartupModule()
{
	UE_LOG(LogChaliceEditor, Display, TEXT("ChaliceEditor: Module started"));

	if (GUnrealEd)
	{
		TSharedPtr<FWeaponTraceVisualizer> WeaponTraceVisualizer = MakeShareable(new FWeaponTraceVisualizer());
		GUnrealEd->RegisterComponentVisualizer(UWeaponTraceComponent::StaticClass()->GetFName(), WeaponTraceVisualizer);
		WeaponTraceVisualizer->OnRegister();
	}
}

void FChaliceEditorModule::ShutdownModule()
{
	UE_LOG(LogChaliceEditor, Display, TEXT("ChaliceEditor: Module shutdown"));

	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UWeaponTraceComponent::StaticClass()->GetFName());
	}
}
