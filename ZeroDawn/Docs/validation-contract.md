# Zero Dawn: Modern Warfare — Validation Contract

This document defines the validation assertions for Milestones 5 (Steam Sessions), 6 (AAA Features: killcam, spectator, grenade, career stats), and 7 (Asset Specs .md files). Each assertion is verifiable by static code analysis (`cpp-review`) and/or file-system inspection.

---

## M5 — Steam Sessions (ID prefix: VAL-SES)

### VAL-SES-001: UZeroDawnSessionSubsystem exists as UGameInstanceSubsystem
A class `UZeroDawnSessionSubsystem` must exist, inheriting from `UGameInstanceSubsystem`, declared with `GENERATED_BODY()`, and placed in the `Multiplayer/` directory (or a dedicated `Sessions/` directory).
**Tool:** cpp-review
**Evidence:** Check that file `ZeroDawnSessionSubsystem.h` exists in `Source/ZeroDawn/`, contains `class UZeroDawnSessionSubsystem : public UGameInstanceSubsystem`, and is marked `UCLASS()`.

### VAL-SES-002: CreateSession() function with online parameters
The subsystem must expose a `CreateSession()` function (BlueprintCallable) that accepts at minimum: `int32 NumPublicConnections`, `FString MapName`, and `FString GameModeName`. It must call `IOnlineSession::CreateSession()`.
**Tool:** cpp-review
**Evidence:** Check for a UFUNCTION `CreateSession` with parameters for max players, map, and game mode. Verify the implementation invokes `Online::GetSessionInterface()` and `SessionInt->CreateSession()`.

### VAL-SES-003: FindSessions() with completion delegate
A `FindSessions()` function must be defined, taking at minimum a search parameter (e.g., `bool bIsLAN`). It must register a delegate `OnFindSessionsComplete` that receives an `FOnFindSessionsComplete` callback with results.
**Tool:** cpp-review
**Evidence:** Check for `FindSessions()` UFUNCTION. Verify declaration of `FOnFindSessionsComplete` delegate and that `SessionInt->FindSessions()` is called with a bound delegate.

### VAL-SES-004: JoinSession() with session ID parameter
A `JoinSession()` function must be defined, accepting a session index or session ID (`int32 SessionIndex` or `FOnlineSessionSearchResult`). It must call `SessionInt->JoinSession()` for the local player.
**Tool:** cpp-review
**Evidence:** Check for `JoinSession(int32 SessionIndex)` or similar UFUNCTION. Verify `SessionInt->JoinSession(0, SessionResult)` is called in the implementation.

### VAL-SES-005: DestroySession() cleanup
A `DestroySession()` function must be defined that calls `SessionInt->DestroySession()` and cleans up the current session state. Must include an `OnDestroySessionComplete` delegate.
**Tool:** cpp-review
**Evidence:** Check for `DestroySession()` UFUNCTION. Verify `SessionInt->DestroySession()` is called and a completion delegate is handled.

### VAL-SES-006: Lobby UI widget with player list, ready state, game mode selection
A lobby UI widget (C++ class or Blueprint) must exist that displays a player list, per-player ready state toggle, and a game mode selection control. It should be shown when a session is created/joined.
**Tool:** cpp-review
**Evidence:** Check for a widget class (e.g., `ULobbyWidget` or `UZeroDawnLobbyWidget`) inheriting `UUserWidget`. Verify it contains bindings for player list, ready state, and game mode selector (UPROPERTY meta = BindWidget).

### VAL-SES-007: Host Game button in main menu creates a session
The main menu UI must include a "Host Game" button that calls `CreateSession()` with configured parameters (max players, map, game mode).
**Tool:** cpp-review
**Evidence:** Check the main menu widget class for a button with `OnClicked` binding that calls a function which invokes `UZeroDawnSessionSubsystem::CreateSession()`.

### VAL-SES-008: Find Game button searches and lists available sessions
The main menu UI must include a "Find Game" button that calls `FindSessions()` and displays results in a list (e.g., `ListView` or `WrapBox`). Selecting an entry should call `JoinSession()`.
**Tool:** cpp-review
**Evidence:** Check the main menu widget for a button bound to `FindSessions()`. Verify session search results are populated into a list widget and the list's `OnItemClicked()` calls `JoinSession()`.

### VAL-SES-009: Session settings include game mode and map name
`CreateSession()` must populate `FSessionSettings` with at least two custom keys: `SETTING_GAMEMODE` and `SETTING_MAPNAME` (or equivalent `FName` keys), set via `Settings.Settings.Add(FName("GAMEMODE"), ...)`.
**Tool:** cpp-review
**Evidence:** Check `CreateSession()` implementation for `FSessionSettings::Add()` calls with game mode and map name keys.

### VAL-SES-010: DefaultEngine.ini Steam settings preserved
The `DefaultEngine.ini` must contain `[OnlineSubsystemSteam]` with `bEnabled=true` and `SteamDevAppId=480`. The `[OnlineSubsystem]` section must have `DefaultPlatformService=Steam`.
**Tool:** cpp-review (file inspection)
**Evidence:** Verify `Config/DefaultEngine.ini` contains:
```
[OnlineSubsystemSteam]
bEnabled=true
SteamDevAppId=480
```
and
```
[OnlineSubsystem]
DefaultPlatformService=Steam
```

---

## M6 — AAA Features (ID prefix: VAL-AAA)

### VAL-AAA-001: Killcam activates on death — camera attaches to killer for ~3 seconds
When the local player dies, the camera must detach from the dead pawn and attach to the killer's viewpoint (or killer's camera) for approximately 3 seconds. This must be handled by a component (e.g., `UZeroDawnKillcamManager`).
**Tool:** cpp-review
**Evidence:** Check for a killcam manager class or component. Verify that on death, `PlayerController->SetViewTarget(KillerActor)` is called with a blend time, and `GetWorldTimerManager().SetTimer()` is used to set a ~3-second duration.

### VAL-AAA-002: Killcam ends properly and triggers respawn sequence
After the 3-second killcam duration expires, the camera must return to the player and trigger the respawn sequence. Input must be re-enabled and HUD shown.
**Tool:** cpp-review
**Evidence:** Check the killcam timer callback. Verify it calls `PlayerController->SetViewTarget(PlayerPawn)` and then initiates respawn (e.g., `RequestRespawn()`). Verify HUD visibility is restored (e.g., `ShowHUDWidget()`) and input is re-enabled.

### VAL-AAA-003: Spectator mode available in S&D after death — cycles through alive teammates
After death in Search & Destroy, the player must enter spectator mode and be able to cycle through alive teammates (next/previous). A spectator pawn or controller override must handle this.
**Tool:** cpp-review
**Evidence:** Check for an `AZeroDawnSpectator` class inheriting `ASpectatorPawn` (or a `APlayerController` override that sets `SpectatorPawn`). Verify `CycleTeamMembers()` or similar next/prev functions exist. Check that S&D game mode transitions dead players to spectator (e.g., `SetViewTarget` on alive teammates).

### VAL-AAA-004: Spectator shows overlay with observed player name/health
The spectator HUD must display an overlay showing the currently observed player's name and health. This overlay must be a HUD widget or a separate spectator widget.
**Tool:** cpp-review
**Evidence:** Check for a spectator overlay widget class (e.g., `USpectatorOverlayWidget`). Verify it reads and displays the observed pawn's `PlayerState->GetPlayerName()` and `CurrentHealth`. Check that it's added to the viewport on spectator enter and removed on exit.

### VAL-AAA-005: AZeroDawnGrenade class exists with projectile movement and timer
A class `AZeroDawnGrenade` must exist, inheriting `AActor`, with a `UProjectileMovementComponent` and an explosion timer set to 4 seconds after spawn.
**Tool:** cpp-review
**Evidence:** Check for `ZeroDawnGrenade.h` containing `class AZeroDawnGrenade : public AActor`. Verify `UProjectileMovementComponent* ProjectileComponent` is present. Verify a `float ExplosionTimer` or `FTimerHandle` configured to 4 seconds in the constructor (e.g., `InitialLifeSpan = 4.0f`).

### VAL-AAA-006: Grenade explodes after 4 seconds dealing radial damage
On timer expiry, the grenade must call a reliable multicast function that deals radial damage via `UGameplayStatics::ApplyRadialDamage()` with configured `ExplosionRadius` and `ExplosionDamage`.
**Tool:** cpp-review
**Evidence:** Check for `MulticastExplode()` or `OnExplosionTimer()` that calls `UGameplayStatics::ApplyRadialDamage()`. Verify `ExplosionRadius` and `ExplosionDamage` properties exist and are used in the radial damage call.

### VAL-AAA-007: Grenade has friendly fire check — doesn't damage own team
The radial damage must use a `FCollisionQueryParams` that checks the instigator's team. Damage should only be applied to actors whose team differs from the grenade's owning player's team (or the `TArray<AActor*> IgnoreActors` must include teammates).
**Tool:** cpp-review
**Evidence:** Check that `ApplyRadialDamage` is NOT called with a blanket ignore; verify that either `bIgnoreFriendlyFire` is used or the `DamagePrevention` channel/team check prevents friendly damage. Look for team comparison logic in the explode function.

### VAL-AAA-008: UZeroDawnWeaponComponent::ThrowGrenade() function exists
`UZeroDawnWeaponComponent` must expose a `ThrowGrenade()` BlueprintCallable function that spawns an `AZeroDawnGrenade` at the character's location and launches it using the projectile movement component.
**Tool:** cpp-review
**Evidence:** Check `ZeroDawnWeaponComponent.h` for `UFUNCTION(BlueprintCallable, Category = "Weapons") void ThrowGrenade();`. Verify the implementation spawns `AZeroDawnGrenade` and sets its initial velocity.

### VAL-AAA-009: Career stats tracked (K/D, wins/losses, playtime) and persisted via SaveGame
A SaveGame class (e.g., `UZeroDawnSaveGame`) must exist inheriting `USaveGame`, containing career stats: total kills, total deaths, wins, losses, playtime. It must implement save/load functions via `UGameplayStatics::SaveGameToSlot()` and `LoadGameFromSlot()`.
**Tool:** cpp-review
**Evidence:** Check for a SaveGame class with `UPROPERTY` fields for Kills, Deaths, Wins, Losses, PlayTime (or equivalent). Verify `SaveToSlot()` and `LoadFromSlot()` or similar methods call `UGameplayStatics::SaveGameToSlot` / `LoadGameFromSlot`. Check that save is triggered at match end.

### VAL-AAA-010: Most used weapon tracked in career stats
The SaveGame must include a map or array tracking per-weapon usage (e.g., `TMap<EWeaponType, int32> WeaponUsageCount` or similar). This must be updated on each kill and displayed in the career stats UI.
**Tool:** cpp-review
**Evidence:** Check SaveGame class for a weapon usage tracking property (`TMap<EWeaponType, int32>` or `TArray<FWeaponUsageStat>`). Verify it's incremented in the kill scoring flow. Verify a career stats UI widget reads and displays the most used weapon.

---

## M7 — Asset Specs (ID prefix: VAL-SPEC)

### VAL-SPEC-001: Asset spec .md files exist for all 8 weapons
Individual `.md` files must exist for each of the 8 weapons: M4A1, AK47, XM4, 9mm, .50GS, MarineSP, SR-25, CQB-9. Each file must reside under a `Docs/Specs/Weapons/` or `AssetSpecs/Weapons/` directory.
**Tool:** cpp-review (file-system inspection)
**Evidence:** Check for files matching patterns such as:
- `**/Specs/Weapons/M4A1*.md`
- `**/Specs/Weapons/AK47*.md`
- `**/Specs/Weapons/XM4*.md`
- `**/Specs/Weapons/9mm*.md`
- `**/Specs/Weapons/50GS*.md`
- `**/Specs/Weapons/MarineSP*.md`
- `**/Specs/Weapons/SR25*.md`
- `**/Specs/Weapons/CQB9*.md`
All 8 files must be present.

### VAL-SPEC-002: Weapon spec includes prompt for image generation + Meshy v6 configuration
Each weapon spec `.md` file must contain two clearly separated sections: an AI image generation prompt (for tools like Midjourney / Stable Diffusion) and a 3D model generation configuration (Meshy v6 or equivalent), specifying style, poly count, and output format.
**Tool:** cpp-review (file inspection)
**Evidence:** Check each weapon `.md` file for sections labeled "AI Image Prompt" (or "Image Generation Prompt") and "Meshy Config" (or "3D Model Config"). Verify Meshy config mentions `Style: Realistic / PBR`, `Poly count: Medium (5k-15k tris)`, and `Format: FBX or GLB`.

### VAL-SPEC-003: Asset spec .md files exist for characters (Alpha soldier, Bravo soldier) with AI image prompt
Individual `.md` files must exist for character assets: Alpha Team soldier and Bravo Team soldier. Each must include an AI image generation prompt describing uniform, pose, gear, and camera composition.
**Tool:** cpp-review (file inspection)
**Evidence:** Check for files matching `**/Specs/Characters/Alpha_Soldier*.md` and `**/Specs/Characters/Bravo_Soldier*.md`. Verify each contains an "AI Image Prompt" section with uniform details (camo pattern, gear, helmet, weapon pose).

### VAL-SPEC-004: Asset spec .md files exist for environments (training map, nuketown, urban, desert)
Individual `.md` files must exist for at least 4 environment/map assets: training map, nuketown, urban street, desert outpost. Each must describe the scene, architectural style, lighting, and reference images.
**Tool:** cpp-review (file inspection)
**Evidence:** Check for files matching `**/Specs/Environments/*.md` or similar. Verify at least 4 environment spec files exist covering training, nuketown/urban, and desert themes. Each must contain an AI image prompt describing the map layout and visual style.

### VAL-SPEC-005: Sound description files exist for all 10+ game sounds
Individual `.md` files (or a single structured `.md` file) must describe at least 10 game sounds (e.g., weapon fire, footsteps, explosion, hitmarker, UI click, killstreak notification, bomb plant/defuse, round start/end, match end, death sound). Each sound must have its own section.
**Tool:** cpp-review (file inspection)
**Evidence:** Check for files matching `**/Specs/Audio/*.md` or `**/Specs/Sounds/*.md`. Verify at least 10 distinct sounds are described.

### VAL-SPEC-006: Sound descriptions include: type, context, emotional quality, duration
Each sound specification must include four fields: `Type` (e.g., "SFX", "Ambient", "UI"), `Context` (when/where it plays), `Emotional Quality` (e.g., "tense", "satisfying", "urgent"), and `Duration` (approximate length in seconds).
**Tool:** cpp-review (file inspection)
**Evidence:** Check each sound entry or file for the presence of `Type:`, `Context:`, `Emotional Quality:` (or `Emotion:`), and `Duration:` fields.

### VAL-SPEC-007: UI asset spec files exist (icons for perks, killstreaks, weapons, HUD elements)
`.md` files must exist specifying UI icons needed: perk icons (10 perks), killstreak icons (5 killstreaks), weapon icons (8 weapons), and HUD element specs (health bar, ammo counter, minimap, crosshair, scoreboard).
**Tool:** cpp-review (file inspection)
**Evidence:** Check for files matching `**/Specs/UI/*.md` or `**/Specs/UI_Icons/*.md`. Verify at least 4 UI spec files or sections covering perk icons, killstreak icons, weapon icons, and HUD elements.

### VAL-SPEC-008: Each .md file clearly separates "AI image prompt" vs "3D model spec" vs "sound description"
Every asset spec `.md` file that includes both 2D/3D assets must use distinct headers to separate AI image generation prompts from 3D modeling parameters. Sound description files must be separate from 3D spec files.
**Tool:** cpp-review (file inspection)
**Evidence:** Check each `.md` file for clear section separation. Weapon/character/environment files must have both `## AI Image Prompt` and `## 3D Model Spec` (or equivalent) headers. Sound files must have `## Sound Description` headers.

### VAL-SPEC-009: Animation spec files exist for required animations (idle, walk, sprint, shoot, reload, inspect)
`.md` files must exist specifying animations for: idle (standing/crouched), walk, sprint, shoot (per weapon type), reload (per weapon type), weapon inspect, death, and slide. Each must describe the animation style, duration, and key poses.
**Tool:** cpp-review (file inspection)
**Evidence:** Check for files matching `**/Specs/Animations/*.md`. Verify at least the following are covered: idle, walk, sprint, shoot, reload, inspect. Each entry must describe the animation purpose, style, and key frames.

### VAL-SPEC-010: Sound descriptions cover weapon-specific sounds for all 8 weapon types
Beyond the 10 general sounds, weapon-specific sound descriptions must exist for each of the 8 weapon types, describing the fire sound character (e.g., "deep boom" for .50 GS, "sharp crack" for SR-25, "suppressed pop" for CQB-9).
**Tool:** cpp-review (file inspection)
**Evidence:** Check sound spec file(s) for entries for each of: M4A1, AK47, XM4, 9mm, .50GS, MarineSP, SR-25, CQB-9 fire sounds. Each must include a unique tonal description distinguishing it from other weapons.
