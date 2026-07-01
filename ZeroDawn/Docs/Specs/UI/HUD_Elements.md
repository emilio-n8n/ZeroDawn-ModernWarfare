# HUD Element Specs

This document contains AI image prompts and technical specifications for the 5 core gameplay HUD elements in Zero Dawn: Modern Warfare. Each spec covers the visual design, layout, states, and technical requirements for these on-screen UI components.

---

## Element 1: Health Bar

### AI Image Prompt
A dynamic health bar widget mockup rendered in a military HUD style. The bar is horizontal, positioned at the bottom-center of the screen, composed of a dark grey (#1A1A2E) background track approximately 200px wide and 16px tall with rounded corners (4px radius). The fill bar uses a gradient from red (#E53935) at low health to green (#4CAF50) at full health, transitioning through orange (#FF9800) at 50%. A subtle white (#FFFFFF) 1px border outlines the bar region. At low health (<30%), a red vignette overlay surrounds the bar area with a pulsing opacity animation. A numeric HP readout ("100") in bold military-style white text (font size 14px, font: monospace or military stencil) is centered over or directly below the bar. The background of the widget is transparent with no decorative elements — pure functional military HUD design. Inspired by Call of Duty and Battlefield health bar layouts.

### Spec
- Format: Widget mockup reference (PNG/screenshot for reference, actual implementation is C++/UMG)
- Resolution reference: 200x16px bar area, full widget ~250x40px
- Style: Minimal military HUD, flat design, no gradients on background
- States:
  - Healthy (75-100%): Solid green fill
  - Moderate (30-74%): Orange fill
  - Critical (1-29%): Red fill with pulsing red screen-edge vignette overlay
  - Empty (0%): Empty bar with dimmed background, death sequence overlay
- Animation: Smooth fill transition (0.1s lerp), critical health pulse at 2Hz
- Color palette: Dark grey track (#1A1A2E), green (#4CAF50), orange (#FF9800), red (#E53935), white text (#FFFFFF)
- Font: Monospace/military stencil, 14px weight, white drop shadow (1px, black, 50% opacity)
- Z-order: Above gameplay but below kill feed and crosshair
- Edge padding: 24px from screen bottom edge

---

## Element 2: Ammo Counter

### AI Image Prompt
An ammo counter HUD widget mockup in military flat style. The widget displays two numbers separated by a forward slash ("30 / 120") in bold white monospace text, positioned at the bottom-right corner of the screen. The current magazine ammo number (left) is larger (18px) and bright white (#FFFFFF), while the reserve ammo number (right) is smaller (14px) and slightly dimmed (#B0BEC5). A small weapon-specific icon (16x16px flat silhouette) sits to the left of the numbers. When the magazine is empty, the current ammo number turns red (#E53935) and pulses. When reloading, a thin horizontal progress bar (80px wide, 4px tall) appears below the numbers, filling from left to right in amber (#FFC107) over the reload duration, with "RELOADING" text in amber 10px font below. The entire widget has a transparent background with no bounding box. Inspired by Call of Duty and Battlefield ammo counter layouts.

### Spec
- Format: Widget mockup reference
- Resolution reference: Widget area ~150x30px
- Style: Minimal military HUD, flat text-based
- Text layout: "[AmmoInMag] / [Reserve]" with weapon icon left
  - Current ammo: 18px bold white (#FFFFFF)
  - Separator: 14px white (#FFFFFF)
  - Reserve ammo: 14px dimmed (#B0BEC5)
- States:
  - Normal: White current ammo
  - Low ammo (<25% of magazine): Current ammo in yellow (#FFC107)
  - Empty magazine: Current ammo red (#E53935), pulsing
  - Reloading: Progress bar in amber, "RELOADING" text below
  - Out of reserve: Reserve ammo shows "0" in red, no slash
- Reload bar: 80x4px, amber fill (#FFC107), 1px white border
- Z-order: Above gameplay, below scoreboard overlay
- Edge padding: 24px from screen bottom edge, 24px from screen right edge
- Font: Monospace (Consolas/Ubuntu Mono), no anti-aliasing for crisp pixel rendering at small sizes

---

## Element 3: Minimap

### AI Image Prompt
A circular minimap/radar widget mockup in military HUD style. The minimap is positioned in the bottom-left corner of the screen, rendered as a circle with a 140px diameter. The map area has a dark semi-transparent background (dark navy #0D1B2A at 75% opacity) with a thin 1px white (#FFFFFF) circular border. Player positions are shown as small colored dots (4px diameter) — friendly players in blue (#2196F3), the local player in bright white (#FFFFFF), and enemies detected by UAV as red dots (#E53935) that fade to transparency over 3 seconds. A directional cone (90-degree arc, 30% opacity white) emanates from the local player dot indicating camera view direction. The top of the minimap always points North (a small "N" indicator, 8px white text, at the top of the circle). Thin grid lines (1px, #FFFFFF, 10% opacity) create a subtle square grid over the map area. The compass rose at top has tick marks every 45 degrees. Inspired by Call of Duty minimap design with circular radar presentation.

### Spec
- Format: Widget mockup reference
- Resolution reference: 140x140px circular area
- Style: Semi-transparent circular radar, military flat design
- Composition:
  - Background: Dark navy (#0D1B2A) at 75% opacity
  - Border: 1px white (#FFFFFF), 100% opacity
  - Grid: Subtle white lines at 10% opacity
  - North indicator: "N" text at top, 8px font
  - Player dots: 4px diameter (blue/white/red)
  - View cone: 90-degree arc, white at 30% opacity
- States:
  - Active (normal operation): Full map with player dots
  - UAV active: Enemy dots visible, fading over 3s
  - Jammed (counter-UAV): Static overlay, no enemy dots, reduced opacity
  - Disabled (in competitive modes): Hidden entirely, or greyed out with "NO SIGNAL" text
- Zoom: Variable zoom level (configurable in settings), default 150m radius
- Z-order: Above gameplay elements, below crosshair and notifications
- Edge padding: 24px from screen left edge, bottom edge aligned with health bar at 24px from bottom
- Update rate: 10Hz for player positions, 2Hz for map tile rendering (if applicable)

---

## Element 4: Crosshair

### AI Image Prompt
A dynamic crosshair HUD element mockup in military tactical style. The crosshair is centered on screen with four inward-pointing brackets (each bracket is 8px long, 2px thick, with a 2px gap from center) rendered in white (#FFFFFF) at 85% opacity. A single center dot (2px diameter) sits at the exact screen center. The brackets spread outward dynamically based on weapon spread value — at maximum spread (hip fire while moving), the brackets extend to 16px from center; at minimum spread (ADS standing still), they contract to 4px from center. The gap between opposing brackets opens and closes smoothly. Below the crosshair center, a small circular hit indicator (6px diameter) flashes red (#E53935) for 0.1s when a shot connects with an enemy. A headshot hit produces a small X shape (two 4px lines crossing at 45 degrees) in red that appears for 0.3s. The crosshair has no background bounding box. Colors: default white (#FFFFFF, 85% opacity), hit indicator red (#E53935), headshot red X (#E53935). Aiming down sights (ADS) reduces the crosshair to just the center dot (no brackets). Inspired by Call of Duty dynamic crosshair system.

### Spec
- Format: Widget mockup reference (rendered via C++ DrawHUD or UMG)
- Resolution reference: ~64x64px area centered on screen
- Style: Minimal dynamic brackets with center dot, tactical flat design
- Bracket specs:
  - Length: 8px (resting), 4px (ADS), 16px (max spread)
  - Thickness: 2px
  - Gap from center: 2px (resting), 1px (ADS), 6px (max spread)
  - Color: White (#FFFFFF) at 85% opacity
- Center dot: 2px diameter, white (#FFFFFF)
- Spread dynamics:
  - Walking: Moderate spread increase (+30%)
  - Sprinting: Maximum spread (brackets fully open)
  - Firing: Spread increases per shot, recovers over time
  - ADS: Brackets contract to just the center dot
  - Crouch/Prone: Reduced spread
- Hit indicators:
  - Hitmarker: Red circle flash, 6px diameter, 0.1s duration
  - Headshot: Red X (two 4px lines at 45°), 0.3s duration
  - Kill confirmation: Red X expands to 8px and fades, 0.5s duration
- Color states:
  - Default: White (#FFFFFF)
  - On enemy target: Red (#E53935) — briefly when aiming at enemy
  - On friendly target: No change or slight blue tint
- Z-order: Topmost HUD element (rendered above all other UI)
- Performance: C++ DrawHUD rendering preferred (no UMG overhead for real-time rendering)

---

## Element 5: Scoreboard

### AI Image Prompt
A full-screen scoreboard overlay mockup in military HUD style. The scoreboard is a semi-transparent dark overlay (full screen, dark #0A0A1A at 80% opacity) with a centered table layout. The table header displays the game mode name in 24px bold white text, centered. Below the header, two team header rows: "ALPHA" in blue (#2196F3) and "BRAVO" in red (#E53935), each with the team's total score in large 20px bold text beside the team name. Below each team header, a table with columns: Rank (#), Player Name, Kills, Deaths, Assists, Score, Ping, Killstreak. Column headers are 12px white bold text with a thin 1px white underline. Player rows alternate between slightly different opacity backgrounds (85% and 75%) for readability. The local player's row is highlighted with a subtle blue tint (#2196F3 at 20% opacity). Each row is 24px tall with 11px white text. At the bottom of the scoreboard, "SCOREBOARD" label in 10px grey text and a hint "Press TAB to close" in 10px grey text. The entire overlay fades in over 0.2s. Inspired by Call of Duty scoreboard layout and visual design.

### Spec
- Format: Widget mockup reference
- Resolution reference: Full-screen 1920x1080 overlay, table area ~800x600px centered
- Style: Semi-transparent full-screen overlay, clean table layout, military flat design
- Overlay background: Full-screen fill, #0A0A1A at 80% opacity
- Table layout:
  - Game mode title: 24px bold white (#FFFFFF), centered at top
  - Team headers: "ALPHA" in blue (#2196F3), "BRAVO" in red (#E53935), score in 20px bold same color
  - Column headers: 12px bold white, underline separator (1px, #FFFFFF, 50% opacity)
  - Columns: #, Player Name, Kills, Deaths, Assists, Score, Ping, Killstreak
  - Player rows: 24px height, alternating opacity (85%/75%)
  - Local player highlight: Blue tint (#2196F3 at 20% opacity)
  - Text: 11px white (#FFFFFF)
  - Font: Monospace/military sans-serif
- States:
  - Active (visible): Full overlay with all player data, animation fade-in 0.2s
  - Hidden: Overlay removed, scoreboard toggle key (TAB)
  - Update: Data refreshes every time the scoreboard is toggled open (or at 1Hz if held open)
  - Empty (no players): "Waiting for players..." text in grey (#757575), centered
- Special indicators:
  - Player with killstreak >= 5: Small flame icon or asterisk next to name
  - Top player (highest score): Crown icon or gold highlight on the name of the best player on each team
  - Party members: Small star icon next to party member names
- Z-order: Above all gameplay HUD elements, below pause menu
- Key bind: TAB to toggle, ESC to close
- Layout width: Max 900px wide, centered with even column distribution

---

## HUD Layout Reference

The following describes the overall HUD screen layout for Zero Dawn: Modern Warfare, referenced at 1920x1080 resolution. All edge positions use 24px padding from the screen edge.

```
┌─────────────────────────────────────────────────────┐
│                                                     │
│                                                     │
│                  [Crosshair]                         │
│                    (center)                          │
│                                                     │
│                                                     │
│                                                     │
│                                                     │
│                                                     │
│  [Minimap]                          [Match Timer]   │
│  140x140 circle                      center top      │
│                                                     │
│                [Health Bar]        [Ammo Counter]   │
│                bottom-center        bottom-right     │
│                                                     │
│                                                     │
│                                                     │
│                [Score Text]                          │
│                above health bar                      │
└─────────────────────────────────────────────────────┘
```

- **Crosshair**: Absolute screen center
- **Minimap**: Bottom-left, 24px from left and bottom edges
- **Health Bar**: Bottom-center, 24px from bottom edge
- **Ammo Counter**: Bottom-right, 24px from right and bottom edges
- **Match Timer**: Top-center, 24px from top edge, 16px bold white text
- **Score Display**: Above health bar (or top-center next to timer), showing Alpha vs Bravo scores
- **Kill Feed**: Top-right, stacked notification entries, auto-fading after 4 seconds
- **Streak Counter**: Below ammo counter (or left of it), showing current killstreak count
- **Low Health Warning**: Full-screen red vignette overlay, pulsing opacity, active when health < 30%
- **Spectator Overlay**: Bottom-center area when spectating, showing observed player name and health
