using UnrealBuildTool;

public class ZeroDawn : ModuleRules
{
	public ZeroDawn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bRequiresImplementModule = false;
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"UMG",
			"Slate",
			"SlateCore",
			"GameplayTags",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"OnlineSubsystemSteam",
			"PhysicsCore",
			"Niagara",
			"GameplayTasks",
			"AnimGraphRuntime"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
