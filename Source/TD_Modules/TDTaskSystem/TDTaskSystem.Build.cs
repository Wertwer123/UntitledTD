using UnrealBuildTool;

public class TDTaskSystem : ModuleRules
{
    public TDTaskSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "TDUnits", 
                "TDPathfinding"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "Building",
                "MiscGameplay", 
                "UntitledTD",
            }
        );
    }
}