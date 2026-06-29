#!/usr/bin/env python3
"""Generate reference images for Meshy prompts"""

import os, math
from PIL import Image, ImageDraw, ImageFont

OUTPUT_DIR = "/teamspace/studios/this_studio/AAA-game/ZeroDawn/Content/ZeroDawn/ReferenceImages"
os.makedirs(OUTPUT_DIR, exist_ok=True)

def draw_weapon_silhouette(draw, cx, cy, color, weapon_type="rifle"):
    """Draw weapon silhouette for reference"""
    if weapon_type == "rifle":
        draw.rectangle([cx-120, cy-8, cx+120, cy+8], fill=color)  # barrel
        draw.rectangle([cx-80, cy-20, cx-20, cy+20], fill=color)   # receiver
        draw.rectangle([cx-80, cy-30, cx-22, cy-22], fill=color)   # carry handle
        draw.rectangle([cx+40, cy-12, cx+100, cy+12], fill=color)  # stock
        draw.rectangle([cx-40, cy+8, cx-20, cy+35], fill=color)    # magazine
        draw.rounded_rectangle([cx-45, cy+22, cx-15, cy+35], radius=5, fill=color)  # grip
    elif weapon_type == "pistol":
        draw.rectangle([cx-50, cy-6, cx+50, cy+6], fill=color)     # barrel
        draw.rectangle([cx-30, cy-15, cx+30, cy+15], fill=color)   # slide
        draw.rectangle([cx-25, cy-25, cx+25, cy-18], fill=color)   # rear sight
        draw.polygon([(cx-10, cy+5), (cx+15, cy+5), (cx+20, cy+30), (cx-5, cy+30)], fill=color)  # grip
        draw.rectangle([cx-8, cy+10, cx+8, cy+25], fill=color)     # magazine
    elif weapon_type == "shotgun":
        draw.rectangle([cx-120, cy-10, cx+120, cy+10], fill=color) # barrel
        draw.rectangle([cx-70, cy-22, cx-10, cy+22], fill=color)   # receiver
        draw.rectangle([cx-50, cy+10, cx-20, cy+38], fill=color)   # tube mag
        draw.rectangle([cx+40, cy-14, cx+100, cy+14], fill=color)  # stock
        draw.rounded_rectangle([cx-65, cy+25, cx-35, cy+38], radius=4, fill=color)  # grip
    elif weapon_type == "sniper":
        draw.rectangle([cx-150, cy-6, cx+150, cy+6], fill=color)   # long barrel
        draw.rectangle([cx-80, cy-22, cx-10, cy+22], fill=color)   # receiver
        draw.rectangle([cx-75, cy-35, cx-15, cy-25], fill=color)   # scope
        draw.rectangle([cx+40, cy-14, cx+110, cy+14], fill=color)  # stock
        draw.rectangle([cx-30, cy+10, cx-10, cy+30], fill=color)   # magazine
        draw.rounded_rectangle([cx-55, cy+25, cx-25, cy+38], radius=4, fill=color)  # grip

def generate_weapon_reference(name, weapon_type, prompt, filename):
    img = Image.new('RGBA', (512, 512), (30, 35, 45, 255))
    draw = ImageDraw.Draw(img)
    
    draw_weapon_silhouette(draw, 256, 240, (220, 100, 80, 255), weapon_type)
    
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 18)
        font_small = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12)
    except:
        font = ImageFont.load_default()
        font_small = font
    
    draw.text((256, 400), name, fill=(255, 255, 255, 255), font=font, anchor="mt")
    draw.text((256, 430), prompt[:60] + "...", fill=(150, 150, 150, 255), font=font_small, anchor="mt")
    draw.text((256, 460), f"Weapon Type: {weapon_type}", fill=(100, 150, 200, 255), font=font_small, anchor="mt")
    
    path = os.path.join(OUTPUT_DIR, filename)
    img.save(path)
    print(f"  ✓ {filename}")

def generate_character_silhouette(draw, cx, cy, color, stance="standing"):
    """Draw soldier silhouette"""
    # Head
    draw.ellipse([cx-12, cy-60, cx+12, cy-36], fill=color)
    # Body
    draw.rectangle([cx-18, cy-35, cx+18, cy+20], fill=color)
    # Arms
    if stance == "standing":
        draw.rectangle([cx-35, cy-30, cx-18, cy+15], fill=color)  # left arm
        draw.rectangle([cx+18, cy-30, cx+40, cy+15], fill=color)   # right arm
        draw.rectangle([cx-20, cy+20, cx-12, cy+60], fill=color)  # left leg
        draw.rectangle([cx+12, cy+20, cx+20, cy+60], fill=color)  # right leg
    elif stance == "aiming":
        draw.polygon([(cx-35, cy-30), (cx-18, cy-25), (cx-18, cy-5), (cx-35, cy+5)], fill=color)
        draw.polygon([(cx+18, cy-30), (cx+50, cy-20), (cx+50, cy+5), (cx+18, cy+5)], fill=color)
        draw.rectangle([cx-20, cy+20, cx-12, cy+60], fill=color)
        draw.rectangle([cx+12, cy+20, cx+20, cy+60], fill=color)
    # Helmet
    draw.arc([cx-14, cy-62, cx+14, cy-42], 180, 0, fill=color, width=3)

def generate_environment_reference(name, prompt, filename, color_scheme="desert"):
    img = Image.new('RGBA', (512, 512), (30, 35, 45, 255))
    draw = ImageDraw.Draw(img)
    
    if color_scheme == "desert":
        wall_c, floor_c, detail_c = (200, 170, 120), (180, 150, 100), (160, 130, 80)
    elif color_scheme == "urban":
        wall_c, floor_c, detail_c = (120, 130, 140), (100, 110, 120), (80, 90, 100)
    else:
        wall_c, floor_c, detail_c = (100, 120, 100), (80, 100, 80), (60, 80, 60)
    
    draw.rectangle([0, 350, 512, 512], fill=(*floor_c, 255))
    draw.rectangle([0, 0, 512, 350], fill=(*wall_c, 255))
    
    draw.rectangle([80, 100, 200, 350], fill=(*detail_c, 255))
    draw.rectangle([312, 50, 432, 350], fill=(*detail_c, 255))
    draw.rectangle([50, 120, 70, 200], fill=(50, 50, 50, 255))
    draw.rectangle([442, 150, 462, 250], fill=(50, 50, 50, 255))
    
    draw.polygon([(200, 100), (250, 50), (300, 100)], fill=(*detail_c, 255))
    
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16)
        font_small = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 11)
    except:
        font = ImageFont.load_default()
        font_small = font
    
    draw.text((256, 460), name, fill=(255, 255, 255, 255), font=font, anchor="mt")
    draw.text((256, 485), prompt[:70] + "...", fill=(150, 150, 150, 255), font=font_small, anchor="mt")
    
    path = os.path.join(OUTPUT_DIR, filename)
    img.save(path)
    print(f"  ✓ {filename}")

# === WEAPONS ===
generate_weapon_reference(
    "M4A1 Assault Rifle", "rifle",
    "Tactical M4 carbine with 14.5 inch barrel, Magpul stock, picatinny rail, foregrip, 30rd magazine, black finish",
    "M4A1_Rifle.png"
)
generate_weapon_reference(
    "AK-47 Assault Rifle", "rifle",
    "Classic AK-47 with wood furniture, 30rd banana magazine, steel receiver, 7.62x39mm, worn battlefield finish",
    "AK47_Rifle.png"
)
generate_weapon_reference(
    "CQB-9 SMG", "rifle",
    "Compact 9mm submachine gun with suppressor, collapsible stock, 35rd mag, rail system, tactical black",
    "CQB9_SMG.png"
)
generate_weapon_reference(
    "Marine SP Shotgun", "shotgun",
    "Pump action 12-gauge tactical shotgun, 8 round tube magazine, synthetic stock, picatinny rail, desert tan",
    "MarineSP_Shotgun.png"
)
generate_weapon_reference(
    "SR-25 Sniper Rifle", "sniper",
    "7.62mm semi-auto sniper rifle with 22 inch barrel, bipod, 10rd magazine, scope with lens covers, dark earth",
    "SR25_Sniper.png"
)
generate_weapon_reference(
    "9mm Pistol", "pistol",
    "Compact 9mm semi-auto pistol, 4 inch barrel, 12rd magazine, polymer frame, steel slide, black",
    "9mm_Pistol.png"
)
generate_weapon_reference(
    ".50 GS Heavy Pistol", "pistol",
    "Large caliber .50 AE semi-auto pistol, 5 inch barrel, 7rd magazine, slide mounted safety, brushed metal",
    "50GS_Pistol.png"
)
generate_weapon_reference(
    "Combat Knife", "pistol",
    "Tactical combat knife with serrated edge, black G10 handle, steel blade, nylon sheath",
    "Knife.png"
)

# === CHARACTERS ===
for i, (name, stance, skin) in enumerate([
    ("Soldier_Alpha_Standing", "standing", "urban"),
    ("Soldier_Alpha_Aiming", "aiming", "urban"),
    ("Soldier_Bravo_Standing", "standing", "desert"),
]):
    img = Image.new('RGBA', (512, 512), (30, 35, 45, 255))
    draw = ImageDraw.Draw(img)
    colors = {"urban": (60, 80, 100), "desert": (140, 120, 80)}
    color = colors[skin]
    alpha_color = (color[0]+40, color[1]+30, color[2]+20)
    draw.rectangle([0, 0, 512, 512], fill=color)
    generate_character_silhouette(draw, 256, 320, (50, 55, 60), stance)
    draw.rectangle([0, 0, 10, 512], fill=alpha_color)
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16)
    except:
        font = ImageFont.load_default()
    draw.text((256, 460), name.replace("_", " "), fill=(255, 255, 255, 255), font=font, anchor="mt")
    
    path = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(path)
    print(f"  ✓ {name}.png")

# === ENVIRONMENTS ===
generate_environment_reference("Urban_Street_Map", "Middle eastern urban street with two-story buildings, rubble, destroyed vehicles, alleyways", "Urban_Street.png", "urban")
generate_environment_reference("Desert_Outpost_Map", "Desert military outpost with concrete walls, watchtowers, sandbags, supply crates", "Desert_Outpost.png", "desert")
generate_environment_reference("Training_Facility", "Modern shooting range with pop-up targets, concrete barriers, tire walls, metal structures", "Training_Facility.png", "urban")

# === PROPS ===
for name, desc, shape in [
    ("Ammo_Crate", "Military ammo crate, olive drab, metal corners, 50cal text", "box"),
    ("Explosive_Barrel", "Red explosive barrel, 55 gallon drum, flammable warning labels", "cylinder"),
    ("Capture_Flag_A", "Blue triangular flag on pole, Alpha team insignia, metal base", "flag"),
    ("Capture_Flag_B", "Red triangular flag on pole, Bravo team insignia, metal base", "flag"),
    ("SND_Bomb", "Rectangular metal briefcase with antenna, red button, military spec", "box"),
    ("Sandbag_Wall", "Stacked tan sandbags, tactical defensive position, three layers high", "wall"),
]:
    img = Image.new('RGBA', (512, 512), (30, 35, 45, 255))
    draw = ImageDraw.Draw(img)
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16)
        font_small = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 11)
    except:
        font = ImageFont.load_default()
        font_small = font
    draw.text((256, 240), name.replace("_", " "), fill=(255, 255, 255, 255), font=font, anchor="mm")
    draw.text((256, 280), desc[:60], fill=(180, 180, 180, 255), font=font_small, anchor="mt")
    draw.text((256, 460), f"Prop: {name.replace('_', ' ')}", fill=(255, 255, 255, 255), font=font, anchor="mt")
    path = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(path)
    print(f"  ✓ {name}.png")

print(f"\n✅ {len(os.listdir(OUTPUT_DIR))} reference images in {OUTPUT_DIR}")
