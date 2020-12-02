using UnrealBuildTool;

public class ChaliceDebug : ModuleRules
{
    public ChaliceDebug(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "GameplayAbilities", "GameplayTags", "GameplayTasks",
            "ChaliceCore", "ChaliceAbilities", "ChaliceGame"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
        
        PublicDefinitions.AddRange(new string[] { "CHALICE_DEBUG" });
    }
}
