// Copyright (c) 2020 Spencer Melnick

using UnrealBuildTool;
using System.Collections.Generic;

public class ChaliceEditorTarget : TargetRules
{
	public ChaliceEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ChaliceCore", "ChaliceAbilities", "ChaliceGame", "ChaliceUI", "ChaliceEditor" } );
        
        if (bBuildDeveloperTools)
        {
            ProjectDefinitions.AddRange(new string[] { "CHALICE_DEBUG" });
            ExtraModuleNames.AddRange(new string[] { "ChaliceDebug" });
        }
	}
}
