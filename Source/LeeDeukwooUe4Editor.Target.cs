// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LeeDeukwooUe4EditorTarget : TargetRules
{
	public LeeDeukwooUe4EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "LeeDeukwooUe4", "ArenaBattleSetting" });
		// ExtraModuleNames.Add("LeeDeukwooUe4");
		// ExtraModuleNames.Add("ArenaBattleSetting");
	}
}
