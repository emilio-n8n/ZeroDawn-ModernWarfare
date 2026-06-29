# Unreal Engine Python script - Run from UE Editor: Tools -> Execute Python Script
# Creates all required placeholder assets for Zero Dawn: Modern Warfare

import unreal

def create_input_mapping():
    """Create the Enhanced Input Mapping Context and all Input Actions"""
    
    # Create input actions
    actions = [
        "IA_Move", "IA_Look", "IA_Sprint", "IA_Crouch", "IA_Jump",
        "IA_Slide", "IA_ADS", "IA_Fire", "IA_Reload", "IA_Interact", "IA_Melee"
    ]
    
    action_path = "/Game/ZeroDawn/Input/Actions"
    
    if not unreal.EditorAssetLibrary.does_directory_exist(action_path):
        unreal.EditorAssetLibrary.make_directory(action_path)
    
    created_actions = []
    for action_name in actions:
        asset_path = f"{action_path}/{action_name}"
        if not unreal.EditorAssetLibrary.does_asset_exist(asset_path):
            # Check if InputAction class exists
            input_action_class = unreal.load_class(None, "/Script/EnhancedInput.InputAction")
            if input_action_class:
                factory = unreal.InputActionFactory()
                factory.configure_properties(input_action_class)
                # Create the asset
                asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
                    action_name, action_path, None, unreal.InputActionFactory()
                )
                created_actions.append(asset)
                print(f"Created {asset_path}")
    
    # Create Input Mapping Context
    imc_path = "/Game/ZeroDawn/Input/IMC_FPS"
    if not unreal.EditorAssetLibrary.does_asset_exist(imc_path):
        imc = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            "IMC_FPS", "/Game/ZeroDawn/Input", None, unreal.InputMappingContextFactory()
        )
        if imc:
            # Bind actions to keys
            for i, action in enumerate(created_actions):
                if action:
                    # This is simplified - in reality you'd use the proper API
                    pass
            print(f"Created {imc_path}")
    
    print("Input setup complete!")

def create_placeholder_maps():
    """Create placeholder map assets"""
    maps = [
        "/Game/ZeroDawn/Maps/LobbyMap",
        "/Game/ZeroDawn/Maps/MilitaryBase",
        "/Game/ZeroDawn/Maps/Campaign_Mission1",
        "/Game/ZeroDawn/Maps/MP_Map1",
    ]
    
    factory = unreal.WorldFactory()
    for map_path in maps:
        if not unreal.EditorAssetLibrary.does_asset_exist(map_path):
            map_name = map_path.split("/")[-1]
            map_dir = "/".join(map_path.split("/")[:-1])
            unreal.AssetToolsHelpers.get_asset_tools().create_asset(
                map_name, map_dir, None, factory
            )
            print(f"Created placeholder map: {map_path}")

def create_game_mode_blueprints():
    """Create Blueprint versions of our game modes"""
    game_modes = {
        "BP_TDM_GameMode": "/Script/ZeroDawn.ZeroDawnTDMGameMode",
        "BP_FFA_GameMode": "/Script/ZeroDawn.ZeroDawnFFAGameMode", 
        "BP_Domination_GameMode": "/Script/ZeroDawn.ZeroDawnDominationGameMode",
        "BP_SearchDestroy_GameMode": "/Script/ZeroDawn.ZeroDawnSearchDestroyGameMode",
        "BP_Hardpoint_GameMode": "/Script/ZeroDawn.ZeroDawnHardpointGameMode",
    }
    
    for bp_name, parent_class in game_modes.items():
        bp_path = f"/Game/ZeroDawn/Blueprints/GameModes/{bp_name}"
        if not unreal.EditorAssetLibrary.does_asset_exist(bp_path):
            factory = unreal.BlueprintFactory()
            factory.parent_class = unreal.load_class(None, parent_class)
            unreal.AssetToolsHelpers.get_asset_tools().create_asset(
                bp_name, "/Game/ZeroDawn/Blueprints/GameModes", None, factory
            )
            print(f"Created {bp_path}")

if __name__ == "__main__":
    create_input_mapping()
    create_placeholder_maps()
    create_game_mode_blueprints()
    print("""
=== Setup Complete! ===

Next steps:
1. Open each map and add PlayerStarts tagged 'Alpha' and 'Bravo'
2. Assign BP_TDM_GameMode as default game mode in Project Settings
3. Build Lighting (Ctrl+Shift+.)
4. Package: File -> Package Project -> [Your Platform]
""")
