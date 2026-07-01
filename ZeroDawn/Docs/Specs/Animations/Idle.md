# Animation: Idle

## Description
- Context: Default standing pose when the player is not moving, not firing, and not performing any action. Also serves as the base/transition state for all other movement animations.
- Style: Military combat-ready stance with weight evenly distributed. The soldier maintains a professional, alert posture with weapon held at high-ready or low-ready position depending on whether ADS (Aim Down Sights) is active.
- Duration: Looping (indefinite while no action input is detected)
- Key poses:
  1. **Neutral Idle (Low Ready)**: Feet shoulder-width apart, knees slightly bent for stability. Torso upright but with a slight forward lean (approx. 5 degrees) indicating readiness. Arms bent at ~90 degrees holding the weapon with both hands, muzzle angled down at ~45 degrees. Head facing forward with slight natural periodic motion (breathing, scanning). Weapon held at chest-to-waist height depending on character's equipment.
  2. **ADS Idle**: Weapon raised to eye level, stock firmly against shoulder, both eyes aligned with the sight/optic. Head tilted slightly forward to sight picture. Elbows tucked in close to torso. Minimal body sway — controlled breathing micro-motion only (subtle rise/fall of shoulders ~2-3cm).
  3. **Breathing micro-loop**: Very subtle chest rise and fall (0.5-1cm), shoulders following naturally. Slight head micro-motions (looking around, scanning). Minimal weight shift between feet (~1cm lateral movement every 3-4 seconds).
- Weapon handling: Weapon held in both hands at all times. Fingers naturally resting near the trigger guard (not on trigger) unless ADS. Magazine well visible from side. Subtle weapon sway breathing motion (1-2 degrees rotation).
- Blend spaces: Use Idle as the base pose for locomotion blend space (Idle/Walk/Sprint).
- Additive layers: Breathing animation should be an additive layer on top of base pose.
- Loop: Yes (seamless loop with no noticeable start/end point)
- Target FPS: 30fps for performance (skeletal animation blending)
