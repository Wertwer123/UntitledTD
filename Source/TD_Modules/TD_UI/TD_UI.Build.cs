using UnrealBuildTool;

public class TD_UI : ModuleRules
{
    public TD_UI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "UMG",
                "UMGEditor",
                "UnrealEd",
                
                "Building", 
                "EnhancedInput",
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
                "TDTaskSystem",
                "TDInterfaces",
                "MiscGameplay",
                "TDUnits"
            }
        );
    }
}