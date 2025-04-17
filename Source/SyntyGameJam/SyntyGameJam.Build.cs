// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SyntyGameJam : ModuleRules
{
	public SyntyGameJam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags" /*"GameplayTasks",*/  });
    }
}
