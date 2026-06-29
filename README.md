# Zero Dawn: Modern Warfare

Un FPS militaire AAA réalisé avec Unreal Engine 5, codé entièrement par IA.

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
├── Character/       # Personnage FPS (mouvement, saut, slide, sprint)
├── Weapon/          # Armes (tir, recul, munitions, accessoires)
├── WeaponSystem/    # Gestion inventaire, attachements
├── AI/              # Ennemis (Behavior Tree, perception, combat)
├── Multiplayer/     # Online (PlayerState, GameState, replication)
├── GameModes/       # TDM, FFA, Domination, S&D, Hardpoint
├── Progression/     # XP, niveaux, killstreaks, perks, battle pass
├── UI/              # HUD, menus, scoreboard, killfeed
├── Audio/           # Sons, musique, voix
└── Interactive/     # Portes, objets destructibles, zones
```

## Modes de jeu

- Team Deathmatch (6v6)
- Free-For-All
- Domination
- Search & Destroy
- Hardpoint

## Prochaines étapes

- [ ] Ajouter les modèles 3D (armes, personnages)
- [ ] Créer les niveaux (solo + multijoueur)
- [ ] Ajouter les sons et la musique
- [ ] Textures et matériaux
- [ ] Animations
