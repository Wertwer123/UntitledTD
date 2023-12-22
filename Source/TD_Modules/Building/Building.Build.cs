using UnrealBuildTool;

public class Building : ModuleRules
{
    public Building(ReadOnlyTargetRules Target) : base(Target)
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
                "MiscGameplay",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                
                "Player", 
                "TD_Helpers",
                "TDTaskSystem",
                "TD_UI", 
            }
        );
    }
}