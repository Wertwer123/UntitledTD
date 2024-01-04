using UnrealBuildTool;

public class TDUnits : ModuleRules
{
    public TDUnits(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "MiscGameplay", 
                "TDInterfaces",
                "TDTaskSystem", 
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
                "TDPathfinding"
            }
        );
    }
}