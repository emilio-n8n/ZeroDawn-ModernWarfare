#!/usr/bin/env python3
"""Generate game textures using procedural generation"""

import os
import math
import random
from PIL import Image, ImageDraw

OUTPUT_DIR = "/teamspace/studios/this_studio/AAA-game/ZeroDawn/Content/ZeroDawn/Textures"

def generate_camo(size=512):
    """Military camouflage pattern"""
    img = Image.new('RGBA', (size, size), (80, 90, 60, 255))
    draw = ImageDraw.Draw(img)
    
    colors = [(60, 70, 40), (90, 100, 70), (50, 55, 35), (70, 80, 55)]
    for _ in range(30):
        cx = random.randint(0, size)
        cy = random.randint(0, size)
        r = random.randint(30, 80)
        color = random.choice(colors)
        draw.ellipse([cx-r, cy-r, cx+r, cy+r], fill=(*color, 255))
    
    # Noise overlay
    for y in range(size):
        for x in range(size):
            if random.random() < 0.05:
                n = random.randint(-15, 15)
                r, g, b, a = img.getpixel((x, y))
                img.putpixel((x, y), (r+n, g+n, b+n, a))
    
    return img

def generate_concrete(size=512):
    """Concrete texture"""
    img = Image.new('RGB', (size, size), (140, 140, 140))
    
    for y in range(size):
        for x in range(size):
            noise = random.randint(-15, 15)
            r = 140 + noise
            g = 140 + noise
            b = 142 + noise
            img.putpixel((x, y), (r, g, b))
    
    # Cracks
    draw = ImageDraw.Draw(img)
    for _ in range(5):
        x = random.randint(0, size)
        y = random.randint(0, size)
        for _ in range(30):
            x += random.randint(-2, 2)
            y += random.randint(-2, 2)
            x = max(0, min(size-1, x))
            y = max(0, min(size-1, y))
            draw.point((x, y), fill=(60, 60, 60))
    
    return img

def generate_metal(size=512):
    """Brushed metal"""
    img = Image.new('RGB', (size, size))
    
    for y in range(size):
        base = 160 + random.randint(-5, 5)
        for x in range(size):
            noise = random.randint(-8, 8)
            r = base + noise
            g = base + noise - 5
            b = base + noise - 10
            img.putpixel((x, y), (r, g, b))
    
    return img

def generate_wood(size=512):
    """Wood texture"""
    img = Image.new('RGB', (size, size), (140, 110, 70))
    
    for y in range(size):
        for x in range(size):
            grain = math.sin(y * 0.05 + math.sin(x * 0.02) * 3) * 20 + random.randint(-5, 5)
            r = 140 + grain
            g = 110 + grain * 0.7
            b = 70 + grain * 0.3
            r = max(0, min(255, int(r)))
            g = max(0, min(255, int(g)))
            b = max(0, min(255, int(b)))
            img.putpixel((x, y), (r, g, b))
    
    return img

def generate_sand(size=512):
    """Sand texture"""
    img = Image.new('RGB', (size, size))
    
    for y in range(size):
        for x in range(size):
            noise = random.randint(-12, 12)
            r = 195 + noise
            g = 175 + noise
            b = 140 + noise
            img.putpixel((x, y), (r, g, b))
    
    return img

def generate_normal_map(diffuse_img):
    """Generate a simple normal map from a diffuse texture"""
    w, h = diffuse_img.size
    normal = Image.new('RGB', (w, h))
    pixels = diffuse_img.load()
    
    for y in range(1, h - 1):
        for x in range(1, w - 1):
            p = pixels[x, y]
            if isinstance(p, int):
                hx = hy = 0
            else:
                hx = pixels[x+1, y][0] - pixels[x-1, y][0]
                hy = pixels[x, y+1][0] - pixels[x, y-1][0]
            nx = hx / 512.0
            ny = hy / 512.0
            nz = math.sqrt(max(0, 1 - nx*nx - ny*ny))
            normal.putpixel((x, y), (
                int((nx * 0.5 + 0.5) * 255),
                int((ny * 0.5 + 0.5) * 255),
                int(nz * 255)
            ))
    
    return normal

def generate_all_textures():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    generators = {
        'T_Camo.png': generate_camo,
        'T_Concrete.png': generate_concrete,
        'T_Metal.png': generate_metal,
        'T_Wood.png': generate_wood,
        'T_Sand.png': generate_sand,
    }
    
    for name, gen in generators.items():
        img = gen()
        path = os.path.join(OUTPUT_DIR, name)
        img.save(path)
        print(f"  ✓ {name} ({img.size[0]}x{img.size[1]})")
        
        # Generate normal map
        normal = generate_normal_map(img)
        normal_path = os.path.join(OUTPUT_DIR, name.replace('.png', '_N.png'))
        normal.save(normal_path)
        print(f"    └ {os.path.basename(normal_path)} (normal map)")
    
    print(f"\n✓ {len(generators)} textures + normal maps in {OUTPUT_DIR}")

if __name__ == '__main__':
    generate_all_textures()
