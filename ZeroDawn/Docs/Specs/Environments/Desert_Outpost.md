# Environment: Desert Outpost

## AI Image Prompt
A photorealistic remote desert military outpost set in a vast rocky desert landscape, viewed from a slightly elevated camera angle looking down into a natural bowl-shaped canyon. The scene is captured during the late afternoon golden hour with warm orange-gold sunlight (around 5:30 PM) casting long dramatic shadows across the terrain. The sky is a rich gradient of deep azure overhead transitioning to hazy orange and amber near the horizon, with thin wispy cirrus clouds catching the golden light.

The outpost consists of a cluster of tactical military structures built against the base of a steep sandstone cliff face that curves around the north and east sides of the location. The central structure is a single-story forward operating base (FOB) command building made of tan sandbag-reinforced concrete with a corrugated metal roof, camouflage netting stretched overhead, and sand-colored Hesco blast barriers forming a perimeter. A radio antenna tower rises approximately 15m from the rooftop, with signal cables trailing down.

To the left (west) is a vehicle depot with two HMMWV military jeeps parked under a camo-netted shelter, plus stacked fuel drums and ammunition crates. To the right (east), a makeshift helicopter landing pad marked by a worn windsock and faded H-pad markings on the packed dirt. The terrain is rugged desert floor with scattered rocks, dry scrub brush, and patches of sun-baked clay. A well-worn dirt track leads into the canyon entrance from the south.

The surrounding canyon walls are layered sedimentary rock in shades of warm terracotta, sienna, and ochre, with visible strata lines and weathered surfaces. A few hardy desert acacia trees and dry brush cluster in small wadi depressions. The atmosphere is hazy with a slight dust suspension in the air, warm and dry. No signs of water. Inspired by real-world US forward operating bases in Afghanistan and the original Call of Duty 4 maps like "Pipeline" and "Backlot." Materials: sandstone, concrete, canvas, corrugated metal, packed dirt, desert vegetation. 8K PBR texture quality.

## 3D Model Spec
- Output Format: FBX
- Texture Resolution: 8K (8192x8192) for terrain and major structures, 4K for props
- Style: Realistic PBR, arid desert military
- Poly count: Medium (5k-10k tris for structures, 1k-3k per vehicle)
- Maps needed: Albedo, Normal, Roughness, Metallic, AO
- Topology: Game-ready, quads preferred
- UVs: Non-overlapping, optimized texel density, triplanar mapping for terrain
- Scale: 1 unit = 1 cm, map dimensions approximately 8000x6000x1000

### Architecture
- FOB Command Building: Single-story concrete/sandbag structure with flat corrugated metal roof and camouflage netting (10m x 8m x 4m)
- Radio tower: Lattice steel tower on roof (15m tall) with whip antenna and signal cables
- Vehicle depot: Open-sided shelter with camo netting roof, 4 support poles (6m x 5m x 3.5m)
- Hesco barrier perimeter: U-shaped blast wall around the FOB (1.5m high, sand-colored fabric walls with visible fill lines)
- Helipad: Dirt clearing with faded H marking, windsock on pole
- Sandbag positions: 4x stacked sandbag fighting positions (knee-high) at perimeter corners

### Terrain
- Canyon floor: Packed dirt/sand with areas of cracked clay, gentle slope gradient
- Canyon walls: Layered sedimentary rock with visible strata, weathered surfaces, small ledges
- Wadi: Dry creek bed with scattered rocks and gravel near south entrance
- Scrub vegetation: Dry desert brush, small acacia trees (3-4x), patches of dry grass

### Props
- HMMWV jeeps: 2x military utility vehicles in tan desert paint, parked under depot
- Fuel drums: 6x 55-gallon olive drab fuel drums stacked near depot
- Ammo crates: 8x wooden ammunition crates stacked near command building
- Sandbags: Individual sandbags for fighting positions (worn tan fabric)
- Windsock: Orange/white striped fabric windsock on 5m pole
- Communication equipment: Satellite dish, radio equipment cases
- Folding tables and chairs: Field furniture inside command building area

### Collision
- Simplified box collision for all structures
- Convex collision for vehicles
- Box collision for Hesco barriers, sandbags, crates, drums
- Terrain collision for canyon walls and ground
- NavMesh-compatible along outpost interior and canyon floor
