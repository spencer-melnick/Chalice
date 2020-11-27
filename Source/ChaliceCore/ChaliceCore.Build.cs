// Copyright (c) 2020 Spencer Melnick

using UnrealBuildTool;

public class ChaliceCore : ModuleRules
{
    public ChaliceCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
