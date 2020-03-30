// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE_DoItProjectEditorTarget : TargetRules
{
	public UE_DoItProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] 
        {
            "UE_DoItProject",

            // AI
            "AIModule",

            // Destructible
            "ApexDestruction",

            // Ability
            "HeadMountedDisplay",
            "GameplayAbilities",
            "GameplayTags",

            // Niagara
            "Niagara",
            "NiagaraCore",
            "NiagaraEditor",
        } );
	}
}
