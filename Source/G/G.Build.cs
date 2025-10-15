// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class G : ModuleRules
{
	public G(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "MovieScene", "LevelSequence", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule",
			"OnlineSubsystemSteam", "OnlineSubsystem", "UMG", "Slate", "SlateCore", "AIModule", "GameplayTasks", "RHI"
		});
	}
}