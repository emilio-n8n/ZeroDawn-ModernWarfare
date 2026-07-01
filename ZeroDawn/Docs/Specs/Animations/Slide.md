# Animation: Slide

## Description
- Context: Slide is initiated when the player presses crouch while sprinting. The character drops into a controlled feet-first slide across the ground surface. Used to rapidly transition into cover, dodge fire, or change stance quickly. Governed by the slide movement system in code.
- Style: Fast, aggressive, athletic. The character transitions from a full sprint into a low-profile sliding posture, maintaining forward momentum briefly before coming to a stop or transitioning to crouch/prone.
- Duration: 0.8 seconds (SlideDuration in code). The slide movement speed is 600 u/s (faster than sprint), with the character slowing to a stop over the duration.
- Key poses:
  1. **Drop to Slide (0.0-0.1s)**: From sprint pose, the character's center of gravity drops rapidly. Both knees bend deeply, the torso leans back slightly (~10 degrees) to shift weight backward. Support hand releases the foregrip briefly and extends forward/downward for balance. The trailing leg (right for right-handed shooters) extends forward, heel making first contact with the ground. The weapon is raised slightly (muzzle up ~15 degrees) to avoid hitting the ground. The head drops lower to maintain sight line.
  2. **Slide Extension (0.1-0.4s)**: Both legs extended forward, body in a low seated/lying-back position. The back is arched slightly backward, with weight supported on the heels and lower back/buttocks sliding along the ground. Both arms hold the weapon up and forward, with elbows bent and weapon angled upward ~20 degrees from horizontal. The head is up, looking forward over the weapon. The character's momentum carries them forward at SlideSpeed (600 u/s). The weapon remains pointed forward and slightly up — the character can fire while sliding (if the weapon supports it).
  3. **Deceleration (0.4-0.7s)**: Forward momentum decreases. The body settles lower toward the ground. Feet may drag more as speed decreases. The weapon lowers slightly toward horizontal. Arms begin to retract for transition to crouch or stand.
  4. **Transition to Crouch (0.7-0.8s)**: The character transitions from slide ending into a crouched position. Legs bend to bring feet under the body, the torso rises back to crouch height, and both hands return to the standard weapon grip. From crouch, the character can stand, remain crouched, or continue moving. The slide code calls StopCrouch() to end sliding state.
- Weapon handling: The weapon remains in both hands throughout the slide. Muzzle is kept pointed in a safe forward direction (upward ~20 degrees) to prevent ground strikes. The character CAN fire while sliding (no gameplay restriction), so the weapon must remain in a usable firing position. In ADS, the sight alignment is maintained where possible — the sight may be harder to use with the body in slide posture.
- Movement speeds from code: SlideSpeed = 600.0 u/s, SlideDuration = 0.8s, SlideCooldown = 1.0s
- Slide mechanics from code:
  - Initiated by: StartSlide() when bIsSprinting and IsMovingOnGround
  - SlideStartTime tracked for elapsed duration check in Tick()
  - ServerSetSliding(true/false) RPC for replication
  - Character exits slide when SlideElapsed >= SlideDuration (0.8s)
  - bIsSliding properly replicated for remote clients
- Blend spaces: Slide is a single non-looping animation montage that plays to completion. No locomotion blending during the slide.
- Additional notes: The slide animation must work on flat surfaces and slight inclines. The weapon first-person animation should show the arms lowering and extending naturally as the body drops. Dust/particle effects should be triggered from the feet/body contact point with the ground for visual feedback. The slide animation is a key movement tech in modern military shooters (popularized by Call of Duty: Modern Warfare 2019).
