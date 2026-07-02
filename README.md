# Zero Dawn: Modern Warfare

Un FPS militaire AAA complet et expédiable réalisé avec Unreal Engine 5, codé entièrement par IA.

## Prérequis

- Unreal Engine 5.7+
- Compte Epic Games
- ~150 Go d'espace disque

## Installation (Mac)

1. Installer Unreal Engine 5.7+ via l'Epic Games Launcher
2. Cloner ce repo
3. Faire un clic droit sur `ZeroDawn.uproject` → "Generate Xcode project"
4. Ouvrir `ZeroDawn.xcworkspace`
5. Build (⌘B) puis Run (⌘R)

## Windows

1. Installer Unreal Engine 5.7+
2. Clic droit sur `ZeroDawn.uproject` → "Generate Visual Studio project files"
3. Ouvrir `ZeroDawn.sln`
4. Build (F7) puis Run (F5)

## Architecture

```
Source/ZeroDawn/
├── Character/       # Personnage FPS (mouvement, slide, sprint, ADS, santé)
├── Weapon/          # Armes (8 types, ballistics, recoil, attachments)
├── WeaponSystem/    # Inventaire, grenades, inspect, camera shake
├── AI/              # Ennemis (Behavior Tree, perception sight/hearing)
├── Multiplayer/     # Online (PlayerState, GameState, sessions Steam)
├── GameModes/       # TDM, FFA, Domination, S&D, Hardpoint (+ bomb mechanics)
├── Progression/     # XP/55 niveaux, Battle Pass 100 tiers, perks, killstreaks
├── UI/              # HUD complet C++, hitmarker, settings, killstreak HUD, match end
├── Audio/           # Sons, musique, voix
├── Interactive/     # Bomb, bomb sites, portes, destructibles
└── LevelGeneration/ # Maps procédurales (Training, Nuketown, Domination, S&D)
```

## Fonctionnalités

- ✅ Mouvement FPS complet (sprint, slide, crouch, ADS, jump)
- ✅ 8 armes (M4A1, AK-47, XM4, 9mm, .50GS, Marine SP, SR-25, CQB-9)
- ✅ Attachments, recoil patterns, headshot detection, damage falloff
- ✅ 5 modes de jeu (TDM 6v6, FFA, Domination, Hardpoint, Search & Destroy)
- ✅ Bombe S&D avec sites A/B, plant/defuse
- ✅ AI ennemis avec Behavior Tree, perception, patrouilles
- ✅ Système de progression (55 niveaux, XP, unlocks)
- ✅ Battle Pass (100 tiers)
- ✅ 10 perks (dont 6 implémentés dans cette mission)
- ✅ Create-a-Class (5 loadouts)
- ✅ Killstreaks (UAV, CarePackage, AttackHelicopter, Gunship, Nuke)
- ✅ Killcam (caméra du tueur pendant 3s)
- ✅ Mode spectateur (S&D, cycle coéquipiers)
- ✅ Grenade à fragmentation avec dégâts de zone
- ✅ HUD complet C++ (health, ammo, crosshair, killfeed, scoreboard, timer)
- ✅ Écran de fin de match avec top players
- ✅ Menu paramètres (graphismes, audio, contrôles)
- ✅ Sauvegarde persistante (progression, loadouts, settings, stats carrière)
- ✅ Multijoueur Steam (sessions, lobby, host/find/join)
- ✅ K/D, wins/losses, temps de jeu, arme la plus utilisée

## Modes de jeu

- Team Deathmatch (6v6)
- Free-For-All
- Domination
- Search & Destroy
- Hardpoint

## Génération des Assets

Les fichiers de spécifications sont dans `ZeroDawn/Docs/Specs/` :
- Weapons/ : 8 fichiers .md (prompts IA + config Meshy v6 8K)
- Characters/ : Alpha et Bravo soldiers
- Environments/ : Training, Nuketown, Urban Street, Desert Outpost
- Audio/ : 21 descriptions sonores dont 8 tirs d'armes
- UI/ : Icônes de perks (10), killstreaks (5), weapons (8), HUD elements
- Animations/ : 8 specs (idle, walk, sprint, shoot, reload, inspect, death, slide)
