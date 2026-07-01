# Animation: Reload

## Description
- Context: Magazine change sequence triggered when the player presses the reload input. Animation plays while the ReloadTime timer counts down in code. Both tactical reload (ammo remaining) and empty reload (bolt locked back) variants needed.
- Style: Military tactical reload — fast, practiced, efficient. Codified movements performed under pressure. The soldier swaps magazines with minimal wasted motion, then chambers a round (empty reload) or slaps the bolt release (tactical reload with bolt forward).
- Duration: ReloadTime = variable per weapon (2.0-3.5 seconds based on weapon stats). Minimum 0.5s floor in code.
- Key poses (Tactical Reload — ammo remaining):
  1. **Drop/Retrieve Magazine**: Weapon lowered to ~45 degrees from firing position, rotated ~30 degrees to the right (for right-handed shooter) to expose the magazine well. Support hand moves to magazine, pressing the magazine release button with thumb. Spent/partial magazine drops free or is retained (depending on operator preference). Support hand grabs new magazine from pouch on vest/plate carrier (typically from left side or front of vest).
  2. **Insert Magazine**: New magazine guided into the magazine well with support hand. A firm upward slap to seat the magazine. The motion is a straight upward push with a palm-slap at the end for auditory confirmation. Support hand visually "slaps" the bottom of the magazine (~20cm upward travel).
  3. **Charge / Bolt Release**: For tactical reload (bolt already forward): Support hand moves to the bolt release paddle (AR-style) or charging handle (AK-style) and slaps it downward to release the bolt forward, chambering a round from the fresh magazine. For AR pattern: thumb presses the bolt catch paddle. For AK pattern: charging handle pulled rearward and released. Support hand then returns to foregrip position.
  4. **Return to Ready**: Weapon raised back to firing position. Both hands return to firing grip. A subtle re-acquisition of sight picture. End of reload sequence.

- Key poses (Empty Reload — bolt locked back):
  1. **Magazine Release**: Weapon held at ~45 degrees with bolt locked back (visible open bolt/empty chamber). Support hand presses magazine release, empty magazine drops out. Hand moves to new magazine on vest.
  2. **Insert Fresh Magazine**: New magazine inserted and seated with firm slap (same as tactical reload pose 2).
  3. **Bolt Release**: With AR-style weapons, support hand slaps the bolt catch/release paddle, sending the bolt forward and chambering a round. With AK-style, charging handle pulled back and released. The weapon may rock slightly from the force of the bolt slamming forward.
  4. **Return to Ready**: Weapon back to firing position. Ready to fire. End of reload.

- Weapon handling: Weapon visibility is crucial — the magazine well, bolt, and ejection port must be clearly visible for convincing reload visuals. The support hand performing the reload should clearly break grip from the foregrip during the sequence. The trigger hand maintains grip on the pistol grip throughout (except for charging handle manipulation on some weapons).
- ReloadTime in code: WeaponStats.ReloadTime (varies by weapon, e.g., M4A1 = 2.2s, min 0.5s with attachment bonuses)
- Montage: Played as UAnimMontage via AnimInstance->Montage_Play(). The weapon code sets an FTimerHandle for ActualReloadTime before the animation plays — animation duration should match ActualReloadTime so the sync is tight.
- Weapon-specific variations:
  - AR pattern (M4A1, XM4): Standard AR-15/M16 reload with bolt catch paddle, forward assist
  - AK pattern (AK47): Rock-and-lock magazine insertion, charging handle on right side of receiver
  - SMG (9mm, CQB9): Straight magazine insertion (no rock), bolt catch on receiver
  - Sniper (SR25): AR-10 pattern, larger magazine, bolt release may be different location
  - Pistol (50GS): Slide lock release, magazine inserted into grip, slide release with thumb
  - Shotgun (MarineSP): Drop tube loading or box magazine depending on model; pump action racked after reload
- Additional notes: The ReloadAnimation UAnimMontage* property in AZeroDawnWeapon references this animation. Reloading interrupts firing and sprinting. The animation should not root-motion the character — place relative.
