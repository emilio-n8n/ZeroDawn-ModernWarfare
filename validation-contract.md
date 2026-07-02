# Validation Contract — Milestones 1 & 2

## Milestone 1: Bug Fixes

### VAL-BUG-001: Slide no longer ends in 1 frame
The `StartSlide()` function must not call `SetTimerForNextTick(this, &AZeroDawnCharacter::StopCrouch)`. The slide duration is governed by the Tick loop which checks `SlideDuration` against elapsed time. If `SetTimerForNextTick` is called, the slide will end on the very next frame, making the `SlideDuration` property irrelevant.
Tool: cpp-review
Evidence: Check `ZeroDawnCharacter.cpp` — the `StartSlide()` method must NOT contain any calls to `GetWorldTimerManager().SetTimerForNextTick(...)` targeting `StopCrouch`. The slide lifecycle must be driven entirely by the elapsed-time check in `Tick()`.

### VAL-BUG-002: bIsSliding properly replicated via ServerSetSliding RPC
The `bIsSliding` boolean must be kept in sync across all clients. The `AZeroDawnCharacter` header must declare a `ServerSetSliding` RPC (`UFUNCTION(Server, Reliable, WithValidation)`) and the implementation must set `bIsSliding`. The property should either be marked `UPROPERTY(Replicated)` with a `DOREPLIFETIME` entry, or be set exclusively through the server RPC path so the server value is authoritative and replicated.
Tool: cpp-review
Evidence: Check `ZeroDawnCharacter.h` for a `ServerSetSliding` RPC declaration. Check `ZeroDawnCharacter.cpp` for its `_Implementation` body that assigns `bIsSliding`. Verify that `bIsSliding` appears in `GetLifetimeReplicatedProps()` via `DOREPLIFETIME`.

### VAL-BUG-003: ConstructorHelpers not used outside constructors
`ConstructorHelpers::FObjectFinder` must only be used inside the class constructor or `PostInitProperties`. Using it in `CacheInputActions()` (called from `BeginPlay()`) is invalid at runtime in packaged builds and will crash. The input action references should either be loaded in the constructor and cached, or loaded via `TSoftObjectPtr` + synchronous load.
Tool: cpp-review
Evidence: Check `ZeroDawnCharacter.cpp` — all `static ConstructorHelpers::FObjectFinder<...>` calls must be moved out of `CacheInputActions()` and into the `AZeroDawnCharacter` constructor body, before `CreateDefaultSubobject` calls. The `CacheInputActions()` method should only assign already-loaded pointers and add the mapping context.

### VAL-BUG-004: Interact() has actual logic
`AZeroDawnCharacter::Interact()` must contain implementation logic (e.g., overlap trace, interaction check, delegate broadcast). An empty body means the interact key does nothing.
Tool: cpp-review
Evidence: Check `ZeroDawnCharacter.cpp` — the `Interact()` function body must not be empty `{}`. It must contain at least a line trace or overlap check for interactable actors, and a call to their interaction interface or delegate.

### VAL-BUG-005: MulticastPlayHitEffect / MulticastPlayDeathEffect have implementations
`MulticastPlayHitEffect_Implementation` and `MulticastPlayDeathEffect_Implementation` must spawn visual/audio effects (particles, sound, camera shake). Empty bodies mean hits and deaths produce no feedback on remote clients.
Tool: cpp-review
Evidence: Check `ZeroDawnCharacter.cpp` — both `AZeroDawnCharacter::MulticastPlayHitEffect_Implementation(FVector, FRotator)` and `AZeroDawnCharacter::MulticastPlayDeathEffect_Implementation()` must have non-empty bodies. At minimum, death should play `DeathSound` and hit should spawn `BloodImpactEffect` and play `HitSound`.

### VAL-BUG-006: S&D PostLogin uses Super::PostLogin not Super::Super::PostLogin
`AZeroDawnSearchDestroyGameMode::PostLogin` must call `Super::PostLogin(NewPlayer)` which resolves to `AZeroDawnGameModeBase::PostLogin` (team assignment + restart). Calling `Super::Super::PostLogin` skips the base-class logic entirely, bypassing team assignment and player restart.
Tool: cpp-review
Evidence: Check `ZeroDawnSearchDestroyGameMode.cpp` — the `PostLogin` body must contain exactly `Super::PostLogin(NewPlayer)`. It must NOT contain `Super::Super::PostLogin(NewPlayer)`.

### VAL-BUG-007: S&D bomb timer uses BombPlantTime not RoundStartTime
The bomb detonation countdown must start from the moment the bomb is planted, not from `RoundStartTime`. When `bBombPlanted` is true, the code must compute elapsed time against a `BombPlantTime` or `BombStartTime` variable set in `OnBombPlanted()`.
Tool: cpp-review
Evidence: Check `ZeroDawnSearchDestroyGameMode.cpp` — the `Tick()` function's bomb-timer branch must compute elapsed time from a variable that is set when `OnBombPlanted()` fires (e.g., a `float BombPlantTime` member), NOT from `RoundStartTime`. Verify a new member `BombPlantTime` exists and is assigned in `OnBombPlanted()`.

### VAL-BUG-008: S&D MatchTimeLimit is per-round, separate from match limit
The S&D game mode must use a distinct per-round time limit variable rather than repurposing the base class `MatchTimeLimit`. The base class's `MatchTimeLimit` drives the overall match timeout and `EndCurrentMatch()`, while S&D needs a separate round timer. Changing `MatchTimeLimit` in the S&D constructor to 120s makes the base-class `Tick()` call `EndCurrentMatch()` after 120s globally, which is incorrect for a multi-round mode.
Tool: cpp-review
Evidence: Check `ZeroDawnSearchDestroyGameMode.h` for a dedicated `RoundTimeLimit` property (float). Check `ZeroDawnSearchDestroyGameMode.cpp` — the round-time check in `Tick()` must use this new `RoundTimeLimit` and NOT the inherited `MatchTimeLimit`. The base class `MatchTimeLimit` must remain at its default (600s) or be handled so it doesn't prematurely end the match.

### VAL-BUG-009: S&D bSwitchedSides actually swaps teams
When `bSwitchedSides` becomes true at halftime, all players' teams must be swapped (Alpha ↔ Bravo). Currently the flag is set to true but no team reassignment occurs. Players on Alpha should move to Bravo and vice versa, and spawn points should swap accordingly.
Tool: cpp-review
Evidence: Check `ZeroDawnSearchDestroyGameMode.cpp` — `StartNewRound()` must, when `CurrentRound > RoundsToWin` and before the first round of the second half, iterate all `AZeroDawnPlayerState` instances and swap `PlayerTeam` between Alpha and Bravo. The flag `bSwitchedSides` must guard this swap so it executes only once.

### VAL-BUG-010: Hardpoint loops or ends when all hardpoints exhausted
When `CurrentHardpointIndex` reaches `AllHardpoints.Num()`, the hardpoint mode must either wrap back to index 0 (loop) or end the match with a winner determined by score. Currently Tick becomes a no-op after the last hardpoint, so no scoring occurs and the match never ends via hardpoint logic.
Tool: cpp-review
Evidence: Check `ZeroDawnHardpointGameMode.cpp` — the `Tick()` branch that handles `CurrentHardpointIndex >= AllHardpoints.Num()` must either call `ActivateNextHardpoint()` again after resetting `CurrentHardpointIndex = 0` (loop) or call `EndCurrentMatch()` with a winner determination based on current scores.

### VAL-BUG-011: Domination/Hardpoint determine winner on time limit
When the match time limit is reached (base class `Tick()` calls `EndCurrentMatch()`), Domination and Hardpoint must declare the team with the higher score as the winner. Currently `EndCurrentMatch()` calls `HandleMatchHasEnded()` without any winner-determination logic specific to these modes.
Tool: cpp-review
Evidence: Check `ZeroDawnDominationGameMode.cpp` and `ZeroDawnHardpointGameMode.cpp` — override `OnMatchEnd()` or `EndCurrentMatch()` to compare `TeamAlphaScore` vs `TeamBravoScore` and broadcast the winning team (or tie). Verify that `HandleMatchHasEnded()` or the match-end flow includes this comparison.

### VAL-BUG-012: Settings audio/controls actually apply (no nullptr, actual PC values set)
`ApplyAudioSettings` must not pass `nullptr` for the SoundMix and SoundClass parameters to `SetSoundMixClassOverride`. A valid `USoundMix` class must be loaded (e.g., via `ConstructorHelpers` in the constructor) and used. `ApplyControlsSettings` must actually set mouse sensitivity and invert Y on the player controller via `PlayerInput->SetMouseSensitivity(...)` and `PlayerCameraManager->bInvertYAxis = bInvertY` (or equivalent), not just set `DefaultFOV`.
Tool: cpp-review
Evidence: Check `ZeroDawnSettingsMenu.cpp` — `ApplyAudioSettings` must use a non-null `USoundMix*` obtained from a constructor-loaded reference. `ApplyControlsSettings` must call `PC->PlayerInput->SetMouseSensitivity(Sensitivity)` or set the appropriate input scaling and set `PC->PlayerCameraManager->bInvertYAxis = bInvertY`. The `DefaultFOV` line must be replaced or supplemented with actual sensitivity/invert-Y application.

### VAL-BUG-013: KillstreakHUD uses Client RPC not Multicast for owner-only
`MulticastShowStreak` and `MulticastNukeDetonated` are declared as `NetMulticast` but only the owning client should receive them. The current workaround (early-return when `!IsLocallyControlled()`) wastes bandwidth on all remote clients. These should be `UFUNCTION(Client, Reliable)` RPCs so only the owning client executes them.
Tool: cpp-review
Evidence: Check `ZeroDawnKillstreakHUD.h` — `MulticastShowStreak` and `MulticastNukeDetonated` must be changed from `UFUNCTION(NetMulticast, Reliable)` to `UFUNCTION(Client, Reliable)`. Update the `_Implementation` names and remove the early-return guard in the `.cpp`.

### VAL-BUG-014: MatchEndSequence handles all controllers, safe timer (TWeakObjectPtr, member handle)
`ShowMatchEnd` must use a member `FTimerHandle` (not a local stack variable) so the timer can be cancelled if the actor is destroyed. The timer lambda must capture a `TWeakObjectPtr<AZeroDawnMatchEndSequence>` to avoid accessing a destroyed object. `ReturnToLobby` must iterate all player controllers (not just `GetFirstPlayerController`) to properly restore input mode for every client.
Tool: cpp-review
Evidence: Check `ZeroDawnMatchEndSequence.h` for a member `FTimerHandle ReturnToLobbyTimerHandle`. Check `ZeroDawnMatchEndSequence.cpp` — `ShowMatchEnd` must use this member handle and the lambda must capture `TWeakObjectPtr<AZeroDawnMatchEndSequence>`. `ReturnToLobby` must use `GetWorld()->GetPlayerControllerIterator()` to set cursor and input mode on all controllers.

---

## Milestone 2: HUD / UI

### VAL-HUD-001: UpdateHealth / UpdateAmmo / UpdateCrosshair / UpdateMatchTimer / UpdateScore have C++ implementations
These five `AZeroDawnHUD` methods must contain concrete logic to update the HUD widget (e.g., cast `HUDWidget` to `UZeroDawnHUDWidget` and call the corresponding `UpdateHealthBar`, `UpdateAmmoDisplay`, etc.). Empty bodies mean the HUD never reflects game state changes.
Tool: cpp-review
Evidence: Check `ZeroDawnHUD.cpp` — `UpdateHealth(float, float)`, `UpdateAmmo(int32, int32)`, `UpdateCrosshair(float)`, `UpdateMatchTimer(const FString&)`, and `UpdateScore(int32, int32)` must all have non-empty implementations. Each should cast `HUDWidget` to `UZeroDawnHUDWidget` and call the appropriately named `BlueprintImplementableEvent` function on it.

### VAL-HUD-002: ShowKillFeed creates and shows kill feed widget entries
`AZeroDawnHUD::ShowKillFeed` must instantiate a widget from `KillFeedWidgetClass`, populate it with the killer/victim names and weapon type, and add it to the viewport with a lifespan or animation. An empty body means kill feed entries never appear.
Tool: cpp-review
Evidence: Check `ZeroDawnHUD.cpp` — `ShowKillFeed(const FString&, const FString&, EWeaponType)` must create a widget via `CreateWidget<UUserWidget>(GetOwningPlayerController(), KillFeedWidgetClass)`, set text/variables on it, and call `AddToViewport()`. There must be a mechanism for automatic removal after a timeout.

### VAL-HUD-003: HUDWidget's C++ functions are implemented in C++ (not just BP stubs)
All functions in `UZeroDawnHUDWidget` are currently declared as `BlueprintImplementableEvent`, which means they have no C++ implementation and rely entirely on Blueprint. Each must be converted to either a `BlueprintCallable` C++ implementation or a `BlueprintNativeEvent` with a default C++ body that manipulates bound UMG widgets directly.
Tool: cpp-review
Evidence: Check `ZeroDawnHUDWidget.h` — the following functions must NOT be `BlueprintImplementableEvent`:
- `UpdateHealthBar`
- `UpdateArmorBar`
- `UpdateAmmoDisplay`
- `UpdateWeaponName`
- `ShowKillNotification`
- `UpdateScoreDisplay`
- `UpdateMatchTimer`
- `ShowLowHealthWarning`
- `HideLowHealthWarning`

Each must be a C++ function (either `BlueprintCallable` or `BlueprintNativeEvent` with a `_Implementation` body) that sets text, fills progress bars, or controls widget visibility directly via bound UMG variables.

### VAL-HUD-004: Hitmarker visual (ShowHitmarker / ShowKillConfirmation) implemented in C++
`ShowHitmarker` and `ShowKillConfirmation` in `UZeroDawnHitmarker` are `BlueprintImplementableEvent` stubs. They must be converted to C++ implementations that spawn/update a hitmarker widget or material overlay on the viewport, with headshot differentiation.
Tool: cpp-review
Evidence: Check `ZeroDawnHitmarker.h` — `ShowHitmarker(bool bHeadshot)` and `ShowKillConfirmation()` must no longer be `BlueprintImplementableEvent`. They must be C++ functions (or `BlueprintNativeEvent` with C++ `_Implementation`) that create/manipulate a hitmarker visual widget or image, and play the corresponding sound cue.

### VAL-HUD-005: KillstreakHUD visual implemented in C++
All visual functions in `UZeroDawnKillstreakHUD` are `BlueprintImplementableEvent` stubs. Each must be converted to C++ that creates and updates a killstreak notification widget with animations and sound.
Tool: cpp-review
Evidence: Check `ZeroDawnKillstreakHUD.h` — these functions must no longer be `BlueprintImplementableEvent`:
- `ShowKillstreakNotification(EKillstreakType, const FString&)`
- `ShowKillstreakProgress(int32, int32)`
- `ShowStreakCounter(int32)`
- `PlayKillstreakSound(EKillstreakType)`
- `ShowNukeWarning(int32)`
- `OnNukeDetonated()`

Each must be implemented in C++ (or `BlueprintNativeEvent` with C++ body) to spawn/manage widget actors, animate text, and play sounds.

### VAL-HUD-006: MatchEndSequence visual implemented in C++
`OnMatchEnd`, `OnShowTopPlayers`, `PlayVictoryMusic`, and `PlayDefeatMusic` in `AZeroDawnMatchEndSequence` are `BlueprintImplementableEvent` stubs. Each must be converted to C++ that instantiates the match-end widget, populates it with scoreboard data, and triggers the appropriate audio.
Tool: cpp-review
Evidence: Check `ZeroDawnMatchEndSequence.h` — these functions must no longer be `BlueprintImplementableEvent`:
- `OnMatchEnd(ETeamType, int32, int32, const TArray<FPlayerScoreData>&)`
- `OnShowTopPlayers(const TArray<FPlayerScoreData>&)`
- `PlayVictoryMusic()`
- `PlayDefeatMusic()`

Each must have a C++ implementation (or `BlueprintNativeEvent` with `_Implementation` body) that creates the `MatchEndWidget` from `MatchEndWidgetClass`, binds data to it, adds it to the viewport, and plays the appropriate sound cue through `UGameplayStatics`.

### VAL-HUD-007: Settings ApplyAudioSettings uses valid SoundMix class (not nullptr)
`ApplyAudioSettings` must pass a valid `USoundMix*` as the second argument to `UGameplayStatics::SetSoundMixClassOverride`. The SoundMix must be loaded in the constructor (e.g., via `ConstructorHelpers::FObjectFinder<USoundMix>`) and stored in a `UPROPERTY`. Passing `nullptr` means no audio adjustment is applied.
Tool: cpp-review
Evidence: Check `ZeroDawnSettingsMenu.h` — there must be a `UPROPERTY()` member of type `USoundMix*` that is initialized in the constructor. Check `ZeroDawnSettingsMenu.cpp` — `ApplyAudioSettings` must use this member (not `nullptr`) when calling `SetSoundMixClassOverride`. Similarly, a valid `USoundClass*` must be provided instead of `nullptr`.

### VAL-HUD-008: Settings ApplyControlsSettings actually sets mouse sensitivity and invert Y on player controller
`ApplyControlsSettings` must set the player controller's mouse sensitivity and invert-Y axis directly, not just modify `DefaultFOV`. The correct approach is to call `PC->PlayerInput->SetMouseSensitivity(Sensitivity)` (or equivalent input scaling) and set `PC->PlayerCameraManager->bInvertYAxis = bInvertY`.
Tool: cpp-review
Evidence: Check `ZeroDawnSettingsMenu.cpp` — `ApplyControlsSettings` must contain calls to set mouse sensitivity (e.g., `Cast<UEnhancedPlayerInput>(PC->PlayerInput)` → set scaling, or directly modify input axis properties) and set invert-Y on the camera manager. The `PlayerCameraManager->DefaultFOV = 90.0f` line must be replaced or supplemented with these actual control modifications.
