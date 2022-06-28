// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LeeDeukwooUe4Target : TargetRules
{
	public LeeDeukwooUe4Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		// ExtraModuleNames.AddRange(new string[] { "LeeDeukwooUe4", "ArenaBattleSetting" });
		ExtraModuleNames.Add("LeeDeukwooUe4");
		ExtraModuleNames.Add("ArenaBattleSetting");
	}
}
