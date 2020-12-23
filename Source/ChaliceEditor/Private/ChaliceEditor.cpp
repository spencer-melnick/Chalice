// Copyright (c) 2020 Spencer Melnick

#include "ChaliceEditor.h"
#include "ComponentVisualizers/WeaponTraceVisualizer.h"
#include "ComponentVisualizers/InteractionComponentVisualizer.h"
#include "ChaliceCore/Components/InteractionComponent.h"
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
		RegisterVisualizer<UWeaponTraceComponent, FWeaponTraceVisualizer>();
		RegisterVisualizer<UInteractionComponent, FInteractionComponentVisualizer>();
	}
}

void FChaliceEditorModule::ShutdownModule()
{
	UE_LOG(LogChaliceEditor, Display, TEXT("ChaliceEditor: Module shutdown"));

	if (GUnrealEd)
	{
		for (const FName& ComponentName : ComponentVisualizerNames)
		{
			GUnrealEd->UnregisterComponentVisualizer(ComponentName);
		}
	}
}


template <typename ComponentClass, typename VisualizerClass>
void FChaliceEditorModule::RegisterVisualizer()
{
	TSharedPtr<VisualizerClass> Visualizer = MakeShareable(new VisualizerClass());
	const FName ComponentName = ComponentClass::StaticClass()->GetFName();
	GUnrealEd->RegisterComponentVisualizer(ComponentName, Visualizer);
	Visualizer->OnRegister();
	ComponentVisualizerNames.Add(ComponentName);
}

