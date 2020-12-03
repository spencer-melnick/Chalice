using UnrealBuildTool;

public class ChaliceEditor : ModuleRules
{
    public ChaliceEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "GameplayAbilities", "GameplayTags", "GameplayTasks",
            "UnrealEd",
            "ChaliceCore", "ChaliceAbilities", "ChaliceGame"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}
