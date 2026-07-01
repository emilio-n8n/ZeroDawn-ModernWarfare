# Animation: Walk

## Description
- Context: Standard movement at walking speed (300 units/s base in-game speed). Used for tactical advance, ADS movement, and quiet approach.
- Style: Tactical military walk — controlled, deliberate steps with weapon at the ready. Movement is quiet but purposeful, with the soldier scanning surroundings while moving. Feet placed heel-to-toe with a slight rolling motion.
- Duration: Looping (based on stride cycle, approximately 0.6s per full step cycle at 300 u/s)
- Key poses:
  1. **Left Foot Forward (Contact)**: Left foot making heel contact with ground forward of body, right foot pushing off behind. Weight transitioning forward. Weapon stays at low-ready (or ADS if aiming), arms stable relative to torso. Upper body tracks slightly counter to lower body for natural gate motion.
  2. **Mid-Stance (Passing)**: Left foot flat on ground under body center, right foot lifting and passing left leg. Arms at natural swing compensation — weapon remains stable, elbows slightly flexed. Spine rotated ~3-5 degrees in opposition to pelvis.
  3. **Right Foot Forward (Contact)**: Right foot makes heel contact forward, left foot pushing off behind. Mirror of key pose 1. Complete one full stride cycle.
  4. **ADS Walk Variation**: Same foot motion but upper body is more forward-leaning, weapon sight aligned with eye level. Elbows more tucked. Head positioned for sight picture. Shorter stride length (~70% of normal walk).
- Weapon handling: Weapon remains in both hands, stable. Barrel angle consistent with forward direction. No weapon swing — weapon should appear fixed relative to upper body (as if strapped or held firmly). Subtle weapon up/down bob matching step cadence (~2cm vertical movement per step).
- Movement speeds from code: WalkSpeed = 300.0 u/s (default UE5 speed units)
- Blend spaces: Part of locomotion blendspace with Idle (0%) and Sprint (100%) — Walk occupies ~15-40% input range.
- Additive layers: Head look-aim offset (aim offset blendspace) can be applied additively for ADS aiming while walking.
- Loop: Yes (seamless, 2-step cycle, symmetric left/right)
- Additional notes: The walk animation must be stylistically distinct from sprint — the character is in control, calm, scanning, not rushed. Use a 1.0x playback speed baseline for 300 u/s and scale with actual movement speed.
