// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UntitledTDTarget : TargetRules
{
	public UntitledTDTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("UntitledTD");
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[] {"Player", "TD_UI", "Building", "TD_Helpers", "TDTaskSystem", "TDInterfaces", "MiscGameplay", "TDUnits", "TDPathfinding", "TDVisualizers" });
	}
}
