// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GEditorTarget : TargetRules
{
	public GEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("G");
	}
}