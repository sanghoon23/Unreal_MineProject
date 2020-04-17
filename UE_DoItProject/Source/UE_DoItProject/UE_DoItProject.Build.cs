// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UE_DoItProject : ModuleRules
{
	public UE_DoItProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "RHI",
            "GameplayTasks",
            "HeadMountedDisplay",
            "UMG",
            "Slate",
            "SlateCore",
            "RenderCore",
            "UnrealEd",
            "LevelEditor",
            "EditorStyle",

            // AI
            "AIModule",
            "NavigationSystem",

            // Destructible, Procedural
            "PhysX",
            "APEX",
            "ApexDestruction",
            "ApexDestructionLib",
            "ProceduralMeshComponent",

            // Ability
            "HeadMountedDisplay",
            "GameplayAbilities",
            "GameplayTags",
        });


		PrivateDependencyModuleNames.AddRange(new string[] 
        {
             // Cable
            "CableComponent",

            // Niagara
            "Niagara",
            "NiagaraCore",
            "NiagaraEditor",
        });
	}
}
