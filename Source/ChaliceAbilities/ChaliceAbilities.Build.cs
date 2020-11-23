// Copyright (c) 2020 Spencer Melnick

using UnrealBuildTool;

public class ChaliceAbilities : ModuleRules
{
    public ChaliceAbilities(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore",
            "GameplayAbilities", "GameplayTags", "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}
