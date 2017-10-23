// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WatsonSdk : ModuleRules
{
	public WatsonSdk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"WatsonSdk/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"WatsonSdk/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Http",
                "Json",
                "JsonUtilities",
                "InputCore",
                "OnlineSubsystem",
                "OnlineSubsystemNull",
                "OnlineSubsystemUtils",
                "Voice",
                "WebSockets",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				//"Slate",
                //"SlateCore",

				// ... add private dependencies that you statically link with here ...	
			}
            );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
