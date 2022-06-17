// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LeeDeukwooUe4 : ModuleRules
{
	public LeeDeukwooUe4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	}
}
