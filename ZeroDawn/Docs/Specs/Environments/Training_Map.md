# Environment: Training Map

## AI Image Prompt
A photorealistic indoor military training facility arena, viewed from a high vantage point looking down at a 45-degree angle to capture the full layout. The space is a large rectangular concrete room (roughly 50m x 50m) with smooth grey concrete floors bearing subtle scuff marks from tactical boots. The walls are painted in a utilitarian light grey with visible cinder block seams, approximately 5m high. Recessed fluorescent industrial lighting bathes the room in an even, cool white light, casting soft shadows across the space.

At the center of the room stands a two-story central structure made of raw unpainted concrete blocks with a flat roof, accessible via corners with cover positions. Scattered around the arena are military-grade sand-coloured Hesco barriers, waist-high concrete walls, and stacked olive-drab supply crates serving as cover points. Two spawn platforms are marked on the ground — one blue (Alpha) and one red (Bravo) at opposite corners — with painted lines on the floor. A catwalk runs along the upper perimeter of the walls behind the cinder block partition walls.

The aesthetic is raw, functional, and slightly worn — like a real spec-ops close-quarters battle (CQB) training facility used by special forces. No windows, minimal detail on walls. Inspired by the Killhouse from Call of Duty: Modern Warfare and the training facility from John Wick. Materials: poured concrete, painted cinder block, industrial steel, rubberized flooring. 8K PBR texture quality.

## 3D Model Spec
- Output Format: FBX
- Texture Resolution: 8K (8192x8192) for floor/walls, 4K for props
- Style: Realistic PBR, industrial military
- Poly count: Low (1k-5k tris for architecture, 100-500 tris per prop)
- Maps needed: Albedo, Normal, Roughness, Metallic, AO
- Topology: Game-ready, quads preferred
- UVs: Non-overlapping, optimized texel density, tileable floor texture
- Scale: 1 unit = 1 cm, map dimensions approximately 5000x5000x500

### Architecture
- Floor: Large tiled plane with concrete material, tileable UVs (256x256 cm tiles)
- Walls: Cinder block textured with visible mortar lines, 500cm high
- Central structure: Two-story hollow concrete box with flat roof (400cm x 400cm x 300cm)
- Catwalk: Steel grating walkway along upper wall perimeter (100cm wide), with railing
- Lighting: Fluorescent strip light fixtures on ceiling, emissive material

### Props (referenced from other spec files)
- Hesco barriers (sand-colored fabric-walled barriers): 4x
- Concrete waist-high walls: 4x (150cm long, 150cm high, 50cm wide)
- Supply crates: 6x (olive drab wooden crates, 80cm x 60cm x 60cm)
- Target dummies: 4x (silhouette paper targets on sliding racks)
- Spawn platform markers: 2x (painted floor panels, blue and red)

### Collision
- Simplified box collision for all architecture
- Convex collision for props
- NavMesh-compatible for AI pathfinding
