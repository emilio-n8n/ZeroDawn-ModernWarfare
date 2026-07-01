# Sound: SR-25 Weapon Fire

## Sound Description
- **Type:** SFX
- **Context:** Plays each time the SR-25 sniper rifle is fired. A semi-automatic 7.62mm DMR/sniper rifle with a fire rate of 90 RPM and 10-round magazine.
- **Emotional Quality:** Authoritative, precise, devastating — the sound of a high-powered marksman rifle delivering surgical elimination from distance.
- **Duration:** 0.40s per shot (including echo)
- **Tonal Characterization:** The SR-25 produces a sharp, piercing, high-velocity "CRACK-BOOM" that is the most distinctive long-range fire sound in the game. The report has two distinct phases: first, the sharp supersonic crack of the 7.62mm NATO round breaking the sound barrier — a whip-like snap that is higher in pitch than the AK-47's report. Second, the deeper muzzle blast that follows milliseconds later — a controlled boom shaped by the muzzle brake's radial porting. The muzzle brake adds a distinctive multi-directional venting sound — a sharp, pressurized hiss that fans out radially. The bolt cycling produces a clean, precise metallic click-clack that is more refined and deliberate than the AK-47's rough clatter. The echo of the SR-25's report carries across the map, announcing the presence of a sniper and instilling caution in enemy players.

## Layering Notes
- **Attack:** Dual-phase transient — first the supersonic crack, then the muzzle blast
- **Sustain:** Short for the crack (5-10ms), longer for the boom (30-40ms)
- **Decay:** Longest of any weapon — 300-400ms+ depending on environment. The echo is a key audio signature
- **Reverb:** Heavy — the SR-25's report should echo across large open spaces, adding to its presence
- **Distance modeling:** The sharp crack should be clearly identifiable at maximum range. The supersonic crack arrives before the muzzle blast at distance, creating a natural audio cue for the two-part report
