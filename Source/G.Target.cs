// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GTarget : TargetRules
{
	public GTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("G");
	}
}