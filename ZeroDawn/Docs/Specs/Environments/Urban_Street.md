# Environment: Urban Street

## AI Image Prompt
A photorealistic narrow urban street in a war-torn Middle Eastern city, viewed from a low camera angle at street level looking down a corridor of damaged buildings. The scene captures the golden hour — warm late afternoon sunlight (around 4:30 PM) streaming through gaps in the building line, casting long dramatic shadows across the street and illuminating floating dust particles. The sky is a gradient of pale blue to hazy yellow-orange near the horizon.

The street is flanked on both sides by 3-4 story mixed-use buildings in a Middle Eastern architectural style: flat roofs, satellite dishes, faded beige and cream stucco facades with visible bullet impacts and shrapnel marks, broken windows with shattered glass remnants, rusted air conditioning units protruding from walls, and Arabic signage hanging crookedly above storefronts. The ground floor storefronts include a shuttered kebab shop, a small grocery with faded Coca-Cola awning, and a carpet/rug store with rolled carpets visible through a broken display window.

The asphalt road surface is cracked and cratered from explosions with visible potholes. An overturned and burned-out civilian car lies on its side against a wall on the right side of the street. Rubble piles, twisted rebar, and debris are scattered along the sidewalks. A damaged city bus is parked diagonally across the street near the far end, serving as a major cover point. Dangling electrical wires hang from a snapped utility pole. The atmosphere is tense, dusty, and lightly smoky, evoking urban combat zones in modern asymmetrical warfare. Inspired by Call of Duty: Modern Warfare's "Clean House" and Battlefield's urban maps. Materials: stucco, concrete, asphalt, glass shards, rusted metal, fabric awnings. 8K PBR texture quality.

## 3D Model Spec
- Output Format: FBX
- Texture Resolution: 8K (8192x8192) for major structures, 4K for props and debris
- Style: Realistic PBR, war-torn Middle Eastern urban
- Poly count: Medium (5k-10k tris per building facade, 1k-3k per prop)
- Maps needed: Albedo, Normal, Roughness, Metallic, AO
- Topology: Game-ready, quads preferred
- UVs: Non-overlapping, optimized texel density
- Scale: 1 unit = 1 cm, map dimensions approximately 6000x4000x800

### Architecture
- Building A (North side, near spawn): 3-story mixed use, beige stucco, rooftop satellite dishes, ground floor shuttered shop
- Building B (North side, mid): 4-story residential, cream facade, balcony with laundry line, broken windows
- Building C (South side, near spawn): 3-story, damaged facade with exposed rebar, collapsed corner, grocery store front
- Building D (South side, mid): 4-story, faded yellow stucco, carpet store on ground floor, fire damage stains
- Building E (North side, far): 3-story end cap, corner building with rounded balcony
- Building F (South side, far): 3-story with rooftop water tank, electrical room visible

### Street Elements
- Road surface: Cracked asphalt with potholes and crater markings, faded lane markings
- Sidewalks: Concrete with rubble piles, varying width (1-2m)
- Curbs: Standard concrete curbstones, some sections broken

### Props
- Burned-out car: Mid-size sedan overturned against wall, charred frame, missing wheels
- Damaged city bus: Large bus diagonally parked, bullet holes, shattered windows, faded route sign
- Utility pole: Snapped wooden pole leaning at 45°, dangling wires
- Rubble piles: 6x scattered debris piles (50-150cm tall) with rebar sticking out
- Street signs: Bent metal signposts with Arabic script
- Trash: Scattered debris (cardboard, metal cans, fabric scraps)
- Oil drums: 3x 55-gallon drums, some rusted, near building corners
- Sandbags: Stacked sandbag positions at key intersections

### Collision
- Simplified box collision for building facades
- Convex collision for bus and overturned car
- Simple box collision for all small props
- NavMesh-compatible along street center and cleared pathways
