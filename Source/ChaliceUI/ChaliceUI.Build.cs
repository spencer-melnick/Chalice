// Copyright (c) 2020 Spencer Melnick

using UnrealBuildTool;

public class ChaliceUI : ModuleRules
{
    public ChaliceUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "DeveloperSettings",
            "GameplayAbilities", "GameplayTags", "GameplayTasks",
            "ChaliceCore", "ChaliceAbilities", "ChaliceGame"
        });
    }
}
