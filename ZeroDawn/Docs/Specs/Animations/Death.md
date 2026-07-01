# Animation: Death

## Description
- Context: Played when the character's health reaches 0 (CurrentHealth <= 0). The character takes fatal damage and ragdolls or plays a death animation. Death may occur from gunfire, explosive damage, environmental hazards, or melee.
- Style: Dramatic, impactful, non-looping. The soldier is hit, reacts to the fatal shot location, and collapses to the ground. The animation should be convincing and respectful — no excessive theatrics. Generic enough to work with any damage direction (front, back, left, right).
- Duration: Approximately 1.5-2.5 seconds (full animation before ragdoll). The character then enters a ragdoll state for the remainder of the respawn timer (RespawnDelay = 5.0s).
- Key poses:
  1. **Impact Reaction (0.0-0.2s)**: Character jerks backward in the direction of the incoming shot. The head snaps back (for headshot) or the torso twists (for body shot). Arms may fling outward momentarily losing weapon grip. The weapon drops or hangs from sling. The trigger hand releases the weapon grip. Knees buckle slightly as the body absorbs the impact. This pose should match the hit direction using a hit reaction blendspace (front/back/left/right).
  2. **Collapse Start (0.2-0.6s)**: The body begins to fall. Knees bend significantly, torso leans backward or forward depending on hit direction. Arms go limp or reach briefly toward the wound location. The head drops forward or back. Weapon is no longer in the character's hands (stays attached via sling or drops to ground). Breathing stops — no chest motion.
  3. **Ground Impact (0.6-1.2s)**: The body makes contact with the ground. For backward falls: shoulder blades and back of head contact first, then the full back/spine, followed by legs. For forward falls: hands/forearms break the fall (if possible), then chest and hips. The impact causes a slight bounce/rebound.
  4. **Resting Pose (1.2-2.5s)**: Character comes to rest in a final death pose on the ground. Common resting positions: face down with arms at side or above head; face up with arms splayed to sides; curled slightly on side. Eyes may be open (staring) or closed. No voluntary movement. The body then transitions to full ragdoll simulation for physics-based interaction.
- Weapon handling: Weapon drops from hands upon death. Can be a separate dropped weapon pickup actor on the ground. The weapon mesh should detach from the character's hand bones and remain as a world prop near the body. Optionally, the weapon can remain attached via a sling to the character's body.
- Death sound: DeathSound plays at the moment of impact. The MulticastPlayDeathEffect() code spawns BloodImpactEffect and DeathExplosionEffect particles at the hit location.
- Headshot vs Body shot variations:
  - Headshot: Head snaps backward more dramatically, hands immediately release weapon, faster collapse (0.1s faster timing)
  - Body shot: Torso twists, hands reach toward wound, slower collapse
  - Explosive death: Character is thrown outward from explosion center, more dramatic arc, limbs splayed
- Ragdoll: After the death animation completes (or immediately, configurable), the character transitions to full body ragdoll via the UE5 physics system. The ragdoll should be active for the full RespawnDelay (5.0s default) and visually blend from the last animation pose to physics simulation.
- Additional notes: Death animation must be playable from any state (idle, walk, sprint, jump). Use a blend_in time of ~0.1s for smooth transition from active gameplay to death. The `Die()` function and `MulticastOnDeath()` RPC trigger this animation on all clients. For first-person view: the camera may follow the head of the ragdoll (death cam) or detach for killcam playback.
