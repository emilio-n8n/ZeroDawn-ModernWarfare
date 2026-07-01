# Environment: Nuketown

## AI Image Prompt
A photorealistic suburban American mock-town street from a high aerial perspective, showing a symmetrical layout of two identical two-story houses facing each other across a central strip of grass and pavement. The scene is set in a bright, sunny desert climate under a clear blue sky with harsh noon sunlight casting sharp shadows. The two houses are built in mid-century American suburban style: pale beige stucco walls with white trim, dark brown shingled roofs, attached garages, and chain-link fenced backyards visible behind each house.

Between the two houses runs a wide asphalt street with faded yellow center line markings and white dashed lane lines. A small central plaza area features a modern-style concrete fountain (dry, non-functional), surrounded by manicured grass patches. Parked on the street are two classic American sedan cars from the 1960s in turquoise and cream colors, one on each side of the street, providing visual cover. Green residential mailboxes and utility poles with transformer boxes line the sidewalk. The sidewalks are concrete with visible expansion cuts.

The entire town is surrounded by a tall perimeter wall painted in a faded grey-green, suggesting a military test facility. Bleachers or observation towers are hinted at beyond the wall. The map is relatively small and walkable end-to-end in about 10 seconds. The aesthetic captures the nostalgic, slightly eerie "nuclear test town" vibe of the original Call of Duty Nuketown. Materials: stucco, asphalt, concrete, dry grass, painted wood, glass. 8K PBR texture quality.

## 3D Model Spec
- Output Format: FBX
- Texture Resolution: 8K (8192x8192) for ground and major structures, 4K for props
- Style: Realistic PBR, mid-century suburban
- Poly count: Low-Medium (3k-8k tris per house, 1k-3k per prop vehicle)
- Maps needed: Albedo, Normal, Roughness, Metallic, AO
- Topology: Game-ready, quads preferred
- UVs: Non-overlapping, optimized texel density
- Scale: 1 unit = 1 cm, map dimensions approximately 4000x4000x600

### Architecture
- House Alpha (West): Two-story residential house with garage, front porch, 2 bedrooms upstairs, living room/downstairs, fenced backyard. Beige stucco with dark brown roof.
- House Bravo (East): Mirror layout identical to House Alpha, same materials and colors.
- Ground: Asphalt road (6m wide) with markings, concrete sidewalks (1.5m wide), grass patches
- Central fountain: Circular concrete fountain structure (3m diameter, 1.5m high) with dry basin
- Perimeter wall: Tall concrete wall (4m high) enclosing the map, painted grey-green

### Props
- Sedan Car (West): 1960s-style 4-door sedan, turquoise/white, parked facing north
- Sedan Car (East): 1960s-style 4-door sedan, cream/white, parked facing south
- Mailboxes: 4x residential metal mailboxes on wooden posts
- Utility poles: 4x wooden poles with crossbars and transformer boxes
- Street signs: Mock street signs on metal poles
- Trash cans: 2x metal trash cans near garage doors
- Bleachers: Seating structures visible outside perimeter wall (background only)

### Collision
- Simplified box collision for buildings (full building footprint)
- Custom convex collision for vehicles
- Simple box collision for fountain, mailboxes, utility poles
- NavMesh-compatible street and sidewalk areas for AI pathfinding
