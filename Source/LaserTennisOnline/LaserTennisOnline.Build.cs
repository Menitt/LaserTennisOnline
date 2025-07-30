// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LaserTennisOnline : ModuleRules
{
	public LaserTennisOnline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PrivateIncludePaths.AddRange(new string [] {"LaserTennisOnline\\GameActors", "LaserTennisOnline\\GameModes", "LaserTennisOnline\\Player", "LaserTennisOnline\\UI", "LaserTennisOnline\\Player\\AI"});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG", "MultiplayerSessions", "OnlineSubsystem", "OnlineSubsystemSteam","AIModule", "Niagara"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
