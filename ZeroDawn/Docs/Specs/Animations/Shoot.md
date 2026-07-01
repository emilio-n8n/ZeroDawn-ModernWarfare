# Animation: Shoot (Fire)

## Description
- Context: Trigger pull animation playing each time the weapon is fired. Can be fired from Idle, Walk, or ADS states. Shows weapon recoil and action cycling.
- Style: Snappy, impactful weapon recoil animation specific to each weapon type. Assault rifles have moderate vertical recoil, SMGs have lighter kick, shotguns have heavy recoil, pistols have sharp muzzle flip. The animation conveys the weapon's power and handling characteristics.
- Duration: 0.1-0.25 seconds per shot (varies by weapon and fire rate). At 700 RPM (M4A1 fire rate = 11.67 rounds/second), each shot cycle is ~0.085s — the recoil animation should complete before the next shot.
- Key poses:
  1. **Pre-fire (Aim)**: Weapon aligned with target/sight picture. Stock firmly in shoulder pocket. Both hands in firing grip — trigger finger indexed along the receiver outside trigger guard (before firing). Cheek weld on stock for ADS shots.
  2. **Firing (Recoil Impulse)**: Weapon recoils backward into the shoulder ~2-4cm and pitches upward ~3-8 degrees (depending on weapon type). The upper body and shoulders absorb recoil with a slight backward rock (~1-2 degrees). The weapon muzzle climbs. For ADS: the sight picture is briefly lost as weapon kicks up, then returns to target.
  3. **Recovery (Settle)**: Weapon returns to pre-fire position smoothly over the remainder of the fire interval (controlled by weapon recoil recovery in code). The stock settles back into shoulder, muzzle drops back to target alignment. For automatic fire, the weapon should settle to a slightly higher resting position after each successive shot (increasing spread).
  4. **Action Cycle**: For non-automatic weapons: Visible bolt/slide cycling. For AK47: bolt carrier group cycles rearward. For 50GS: heavy slide moves back and forward. For MarineSP (shotgun): pump action cycling. For M4A1: charging handle and ejection port cover — brass casing ejection visualized.
- Weapon handling: Trigger pull visible (finger moves to trigger, then returns to indexed position). Strong hand grip tightens momentarily during recoil. Support hand provides counter-pressure to stabilize.
- Fire rate in code: WeaponStats.FireRate (e.g., 700 RPM for M4A1 = ~0.086s between shots)
- Blend spaces: Not a loop — single-shot animation that can blend back to Idle/ADS/Walk. For automatic weapons, the animation must be able to "loop" at fire rate speed without resetting to pre-fire each time (use a recoil settle state machine).
- Weapon-specific variations:
  - Assault Rifles (M4A1, AK47, XM4): Moderate recoil, ~5 degree muzzle rise, 0.12s settle time
  - SMGs (9mm, CQB9): Light recoil, ~3 degree muzzle rise, 0.08s settle time, faster recovery
  - Sniper (SR25): Heavy recoil, ~8 degree muzzle rise, 0.35s settle time, bolt action cycling visible
  - Pistol (50GS): Sharp muzzle flip, ~10 degree rise, 0.2s settle, slide cycles visibly
  - Shotgun (MarineSP): Heavy recoil, ~6 degree rise, 0.3s settle, pump action after each shot
- Additional notes: For first-person arms, the animation is limited to the hands/forearms and weapon. For third-person, the full upper body reacts. Separate first-person and third-person animations may be needed, or use a single animation with different camera offsets. The FireAnimation UAnimMontage* property in AZeroDawnWeapon references this animation.
