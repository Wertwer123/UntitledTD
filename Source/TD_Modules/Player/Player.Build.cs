using UnrealBuildTool;

public class Player : ModuleRules
{
    public Player(ReadOnlyTargetRules Target) : base(Target)
    {
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "EnhancedInput",
                "Engine", 
                "TDInterfaces",
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
                
                "TDTaskSystem", 
                "TD_UI",
            }
        );
    }
}