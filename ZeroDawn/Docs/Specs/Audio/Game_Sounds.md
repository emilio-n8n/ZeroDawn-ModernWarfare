# Sound: Generic Weapon Fire

## Sound Description
- **Type:** SFX
- **Context:** Used as a fallback weapon fire sound when a specific weapon's fire sound asset is not loaded, or for ambient distant combat audio that doesn't require weapon-specific differentiation.
- **Emotional Quality:** Neutral, functional — conveys standard combat activity without drawing specific attention.
- **Duration:** 0.15s per shot
- **Tonal Characterization:** A generic but realistic gunshot sound — medium-pitched crack with moderate low-end presence. It should sit comfortably between a rifle and pistol report, with a clean transient and standard metallic bolt cycling tail. This is the audio safety net ensuring no weapon is ever silent even if its specific sound asset is missing.

---

# Sound: Footsteps

## Sound Description
- **Type:** SFX
- **Context:** Plays whenever the player (or other players) take a step while moving. Different variations exist for walking, running, sprinting, and crouching, and the sound changes based on the surface material being traversed (concrete, dirt, metal, wood, gravel, water).
- **Emotional Quality:** Grounding, positional, tactical — provides essential spatial awareness for both the player and nearby enemies. Footsteps are a critical gameplay audio cue for locating opponents.
- **Duration:** 0.15s - 0.35s per step (depending on movement speed and surface)
- **Tonal Characterization:** The footstep sound should be a short, impactful percussive event that clearly communicates the surface material. Concrete steps are hard and sharp with a brief reverberation. Dirt/gravel steps have a crunching, shifting texture with multiple small impacts. Metal steps ring with a hollow, resonant quality. Wood steps produce a solid, mid-frequency thud with slight creaking. Water steps are splashy with liquid displacement sounds. The volume and intensity scale with movement speed: walking produces soft, measured steps; sprinting produces loud, heavy footfalls with clothing rustle. Enemy footsteps should be distinguishable from friendly footsteps by slightly different acoustic treatment and directional audio panning.

## Layering Notes
- **Attack:** Instant percussive impact per surface type
- **Sustain:** Minimal — the initial impact is the main event
- **Decay:** Surface-dependent — concrete rings briefly, dirt is dead, metal resonates
- **Reverb:** The environment should subtly color the footstep reverb (e.g., footsteps echo in corridors)
- **Left/right foot:** Alternate left/right footstep samples to avoid mechanical repetition

---

# Sound: Explosion

## Sound Description
- **Type:** SFX
- **Context:** Plays when a grenade detonates, a bomb explodes (S&D), or any explosive barrel/object is destroyed. The explosion sound is one of the most impactful audio events in the game.
- **Emotional Quality:** Devastating, room-shaking, disorienting — communicates immediate danger and area-denial. A well-timed explosion sound signals a significant battlefield event.
- **Duration:** 0.8s - 1.5s (depending on distance and environment)
- **Tonal Characterization:** A multi-layered sound with three distinct phases: (1) Initial sharp, piercing crack of the explosive detonator and shockwave formation — a high-frequency snap that arrives instantly. (2) The main blast: a deep, rumbling "BOOM" with massive low-frequency content that should physically resonate through audio systems (sub-bass extension down to 20-40Hz). The main blast has a thick, churning quality with debris particle sounds (gravel, shrapnel, dust) embedded in the tail. (3) The echo and debris fall: a long receding rumble with dirt, gravel, and debris pattering down around the blast zone. The explosion should sound different based on environment — contained and crushing indoors, wide and rolling in open spaces.

## Layering Notes
- **Attack:** Extreme transient — the highest instantaneous peak of any game sound
- **Sustain:** Sustained low-frequency roar for 200-400ms
- **Decay:** Long environmental decay — up to 1-2 seconds depending on space
- **Sub-bass:** Critical — the explosion must have significant content below 60Hz for the visceral "chest punch" feeling
- **Distance modeling:** The initial crack attenuates faster than the low-frequency rumble. Distant explosions sound like low rolling thunder

---

# Sound: Hitmarker

## Sound Description
- **Type:** SFX (UI)
- **Context:** Plays instantly when the player's bullet connects with an enemy target. A headshot hitmarker sound should be distinct from a body shot hitmarker. A kill confirmation sound plays when the shot eliminates the target.
- **Emotional Quality:** Satisfying, rewarding, reinforcing — the hitmarker sound is the primary positive feedback loop for the player's aim. A well-designed hitmarker sound is essential for the game's "feel."
- **Duration:** 0.08s (standard hit), 0.15s (headshot), 0.25s (kill confirmation)
- **Tonal Characterization:** The standard hitmarker should be a quick, clean, two-tone metallic "ding-thwip" — short enough to not interfere with gameplay but distinctive enough to register immediately. The headshot hitmarker adds a third higher-pitched tone to the sequence — a crisp "ding-ding-thwip" that signals the more impactful hit. The kill confirmation is a more substantial ascending two-tone chime that conveys finality and reward — a satisfying "ding-DING" that triggers dopamine response. All hitmarker sounds should sit in the mid-high frequency range to cut through combat noise without being harsh.

---

# Sound: UI Click

## Sound Description
- **Type:** UI
- **Context:** Plays whenever the player navigates menus, presses buttons, toggles settings, or selects options in the UI (main menu, settings menu, pause menu, class selection, lobby).
- **Emotional Quality:** Responsive, tactile, precise — provides audio confirmation that the UI is responding to player input. Good UI sounds make menus feel polished and responsive.
- **Duration:** 0.04s - 0.08s per click
- **Tonal Characterization:** A short, crisp, military-themed click sound. The primary click should be a clean, mid-frequency "tick" with a slight metallic resonance — reminiscent of mechanical switches or tactical equipment. Navigation/hover sounds are softer and lower-pitched. Selection/confirm sounds are more substantial with a slightly longer decay. Back/cancel sounds are a slightly lower-pitched variant to differentiate from forward actions. The overall sonic palette should feel tactical and modern, avoiding anything that sounds "plastic" or "toy-like."

---

# Sound: Killstreak Notification

## Sound Description
- **Type:** UI (in-game HUD)
- **Context:** Plays when the player earns a killstreak reward (UAV, Airstrike, Chopper, Gunship, Nuke). The sound escalates in intensity with each higher-tier killstreak, building toward the ultimate Nuke notification.
- **Emotional Quality:** Triumphant, escalating, empowering — each killstreak notification should make the player feel their consecutive kills are being rewarded with increasing fanfare.
- **Duration:** 1.0s - 2.5s (depending on killstreak tier)
- **Tonal Characterization:** Each killstreak tier has a unique escalating fanfare: (1) UAV (3 kills) — A short, subtle rising two-note chime that signals "reward earned" without being distracting. (2) Airstrike (5 kills) — A more substantial three-note ascending sequence with a brass-like timbre and a subtle explosion rumble tail. (3) Chopper (7 kills) — A dramatic rising five-note fanfare with rotor-blade whoosh effects layered underneath. (4) Gunship (10 kills) — A powerful orchestral hit with heavy low-end, layered with the sound of heavy rotor blades and cannon fire. (5) Nuke (25 kills) — The ultimate killstreak sound: an earth-shaking, multi-layered announcement that begins with a deep sub-bass pulse, rises through a cinematic orchestral swell, and climaxes with a massive impact. All killstreak notification sounds should announce the streak name through a synthesized or processed voice line.

## Voice Line Note
Each killstreak notification should include a processed voice line announcing the streak type (e.g., "UAV inbound," "Airstrike ready," "Nuke imminent!") with appropriate audio processing (radio effect, reverb,轻微的 distortion).

---

# Sound: Bomb Plant

## Sound Description
- **Type:** SFX
- **Context:** Plays in Search & Destroy mode when the attacking player begins planting the bomb at a bomb site. The sound plays during the 5-second plant interaction and completes with a final confirmation beep.
- **Emotional Quality:** Tense, deliberate, high-stakes — the bomb plant sound builds anticipation and alerts nearby defenders that the objective is being activated.
- **Duration:** 5.0s (full interaction), with a final 0.5s confirmation beep
- **Tonal Characterization:** A continuous low-pitched electronic hum or beep that intensifies as the plant progresses. The sound should have a mechanical, clicking quality — like a device powering up and initializing. Each second of the 5-second plant produces a progressively faster and higher-pitched beep, creating urgency. The final confirmation beep is a solid, definitive tone that holds for 0.5 seconds — signaling that the bomb is now live and the 45-second detonation timer has begun.

---

# Sound: Bomb Defuse

## Sound Description
- **Type:** SFX
- **Context:** Plays in Search & Destroy mode when a defending player begins defusing the planted bomb. The sound plays during the 7-second defuse interaction and completes with a different confirmation tone.
- **Emotional Quality:** Urgent, critical, race-against-time — the defuse sound creates tension as the defender works to disable the bomb before detonation.
- **Duration:** 7.0s (full interaction), with a final 0.3s success tone
- **Tonal Characterization:** A descending electronic tone sequence — the opposite trajectory of the plant sound. Each second produces a beep that descends slightly in pitch, creating a sense of working against the clock. Between beeps, a subtle clicking and wire-snipping sound plays — the mechanical act of disarming. The final success tone is a clean, definitive two-note descending chime that signals the bomb has been successfully disabled and the round is won. If the defuse is interrupted (player moves or is killed), the sound cuts out immediately.

---

# Sound: Bomb Explode

## Sound Description
- **Type:** SFX
- **Context:** Plays in Search & Destroy mode when the 45-second bomb timer expires and the bomb detonates. This is the round-ending event for attacking victory.
- **Emotional Quality:** Cataclysmic, final, map-shaking — the bomb explosion is the most impactful single sound event in the game, signaling the end of the round and an attacking team victory.
- **Duration:** 2.0s - 3.0s
- **Tonal Characterization:** A layered, cinematic explosion that builds from the final bomb timer beeps. The detonation begins with a sharp, high-pitched electrical whine (the bomb's internal systems failing), immediately followed by a massive, earth-shaking low-frequency blast. The explosion should have a distinctive nuclear/munition character — a thick, churning fireball sound with debris chunks flying outward. The reverb tail should echo across the entire map, with a ringing silence that follows — a brief moment of audio emptiness before the round-end announcement.

---

# Sound: Round Start

## Sound Description
- **Type:** SFX
- **Context:** Plays at the beginning of each round in round-based modes (Search & Destroy, Domination, Hardpoint) after the pre-match countdown reaches zero. A shorter variant plays at the start of subsequent rounds within a match.
- **Emotional Quality:** Anticipatory, activating, focused — signals that the tactical phase is over and combat is about to begin.
- **Duration:** 1.5s - 2.0s
- **Tonal Characterization:** A military-style horn or buzzer that signals "round start." The first round of the match uses a longer, more dramatic version — a deep, resonant air-horn tone with a slight rise in pitch. Subsequent rounds use a shorter, sharper buzzer that communicates "action resumes." A synthesized voice line ("Round {number} — fight!") should immediately follow the starter tone. The sound should instill a sense of urgency and readiness.

---

# Sound: Round End

## Sound Description
- **Type:** SFX
- **Context:** Plays when a round concludes in round-based game modes. The sound differs based on whether the player's team won or lost the round.
- **Emotional Quality:** Victory: triumphant, empowering, relieving. Defeat: somber, deflating, motivating.
- **Duration:** 2.0s - 3.0s
- **Tonal Characterization:** The round-end sound has two variants: (1) Round Win — An ascending, triumphant brass-and-drums fanfare that rises in pitch and intensity, ending on a satisfying resolution. The sound conveys "mission accomplished" and reinforces team success. (2) Round Loss — A descending, somber tone sequence — a low brass note that fades into silence. Brief and respectful, conveying that the round was lost but the match continues. Both variants include a voice line announcing the outcome (e.g., "Round won!" or "Round lost.").

---

# Sound: Match End

## Sound Description
- **Type:** SFX / Music
- **Context:** Plays when the match concludes after all rounds are played or the score limit is reached. This is the final audio event before the match-end sequence UI appears.
- **Emotional Quality:** Ultimate: euphoric for victory, crushing for defeat. The match-end sound delivers the emotional climax of the entire game session.
- **Duration:** 4.0s - 6.0s
- **Tonal Characterization:** Two dramatically different variants: (1) Match Victory — A full, cinematic orchestral swell with triumphant brass, rolling timpani drums, and a soaring string melody. The sound builds to a glorious climax that makes the player feel like they've achieved something significant. The final chord resolves with a sense of completion and glory. A voice line announces "Victory!" with stadium-announcer energy. (2) Match Defeat — A somber, cinematic orchestral piece with low strings and minor-key brass. The tone is respectful and dramatic, conveying that the battle was hard-fought but ultimately lost. A quiet, processed voice line announces "Defeated..." with a sense of gravity. The sound fades into the match-end UI music loop.

---

# Sound: Death

## Sound Description
- **Type:** SFX
- **Context:** Plays when the player's character dies (health reaches zero). The sound is heard from the first-person perspective — the player's own death sound.
- **Emotional Quality:** Sudden, impactful, disorienting — the death sound should be a jarring audio cue that clearly communicates "you are dead" and immediately shifts the player's mindset from combat to respawn/spectator mode.
- **Duration:** 0.3s - 0.8s
- **Tonal Characterization:** A multi-layered death sound: (1) The impact — a sharp, wet thud with a brief bone-crack texture, synchronized with the hit that killed the player. (2) The vital signs fade — a rapid descending tone (heartbeat deceleration or electronic flatline) that drops in pitch over 0.3-0.5 seconds. (3) Optional: a brief muffled ringing (tinnitus effect) that simulates the auditory shock of fatal trauma. The overall sound should be sudden and unmistakable, cutting through all other audio. The death sound should be slightly different for headshot deaths (sharper impact, more pronounced ringing) versus body shot deaths (duller thud, less ringing). A final brief heartbeat stop provides a clear "death confirmed" audio cue.

## Voice Line Note
A death grunt or exhalation sound from the character should be layered at a lower volume to humanize the death event.
