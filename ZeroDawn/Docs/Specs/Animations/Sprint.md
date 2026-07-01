# Animation: Sprint

## Description
- Context: Full-speed forward running (550 units/s in-game sprint speed). Activated by pressing the sprint input. Cannot fire while sprinting (weapon lowered).
- Style: Full tactical sprint — urgent, aggressive forward motion with weapon lowered to reduce air resistance and improve running form. The soldier is focused on covering ground quickly, not on combat readiness.
- Duration: Looping (approximately 0.5s per full step cycle at 550 u/s)
- Key poses:
  1. **Left Foot Strike**: Left foot strikes ground forward of body, right leg extended back behind with knee bent. Upper body leaning forward ~15-20 degrees for momentum. Both hands gripping weapon which is angled diagonally down-right (for right-handed shooter) with muzzle pointed toward the ground ~45 degrees forward. Weapon stock tucked under right armpit, foregrip hand extended forward.
  2. **Recoil / Push-Off**: Left foot flat under body pushing off, right knee driving forward and up (high knee, ~90 degrees). Opposite arm (left) swings forward with weapon, right arm (trigger hand) stays closer to body. Maximum forward lean.
  3. **Right Foot Strike**: Right foot strikes ground forward, left leg extended back. Symmetrical to key pose 1 but opposite side. Weapon on left side of body now (as left arm is forward).
  4. **Transition (Sprint Start)**: From Idle/Walk, character visibly transitions weapon from low-ready to sprint-carry position over ~0.15s. Weapon lowers from horizontal to diagonal-down, elbows bend to bring weapon closer to body, shoulders hunch forward slightly.
- Weapon handling: Weapon held diagonally across body with barrel angled toward ground. Both hands on weapon but grip is more relaxed than combat grip — elbows are at ~90 degrees, weapon nestled against torso. The weapon visibly bobs with each stride (~5-8cm vertical, ~3-5cm lateral).
- Movement speeds from code: SprintSpeed = 550.0 u/s
- Blend spaces: Sprint is the upper end of the locomotion blendspace (70-100% input range).
- Additive layers: No ADS additive during sprint (sprinting with ADS is not allowed). Head may have gentle tracking motion.
- Loop: Yes (seamless, 2-step cycle)
- Recovery: Sprint-to-fire transition animation must allow weapon to raise from sprint-carry to firing position in 0.3s (SprintToFireTime = 0.3s from code). This transition should be a separate blend or montage that brings the weapon up smoothly.
- Additional notes: The weapon mesh should visibly lower during sprint. The camera FOV may also widen slightly (weapon-only animation, camera FOV widening done in code). Breathing is heavier and more pronounced than walk/idle.
