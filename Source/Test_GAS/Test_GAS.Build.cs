// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Test_GAS : ModuleRules
{
	public Test_GAS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"GameplayAbilities", "GameplayTags", "GameplayTasks",	// GAS 관련
			"UMG", "Slate", "SlateCore",							// 위젯 관련
			"EnhancedInput",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
