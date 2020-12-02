// Copyright (c) 2020 Spencer Melnick

using UnrealBuildTool;
using System.Collections.Generic;

public class ChaliceTarget : TargetRules
{
	public ChaliceTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ChaliceCore", "ChaliceAbilities", "ChaliceGame" } );
    }
}
