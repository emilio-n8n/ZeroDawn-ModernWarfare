#!/usr/bin/env python3
"""Generate basic 3D models (OBJ format) for weapons and props"""

import os
import math

OUTPUT_DIR = "/teamspace/studios/this_studio/AAA-game/ZeroDawn/Content/ZeroDawn/Meshes"

def write_obj(filename, vertices, faces):
    with open(filename, 'w') as f:
        f.write("# Zero Dawn: Modern Warfare - Generated Model\n")
        for v in vertices:
            f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
        for face in faces:
            f.write("f " + " ".join(str(i+1) for i in face) + "\n")
    print(f"  ✓ {filename} ({len(vertices)} verts, {len(faces)} faces)")

def box(x, y, z):
    """Generate a box centered at origin"""
    hx, hy, hz = x/2, y/2, z/2
    verts = [
        (-hx,-hy,-hz), ( hx,-hy,-hz), ( hx, hy,-hz), (-hx, hy,-hz),
        (-hx,-hy, hz), ( hx,-hy, hz), ( hx, hy, hz), (-hx, hy, hz),
    ]
    faces = [
        (0,1,2,3), (4,5,6,7), (0,1,5,4),
        (2,3,7,6), (0,3,7,4), (1,2,6,5)
    ]
    return verts, faces

def cylinder(radius, height, segments=16):
    """Generate a cylinder"""
    verts = [(0, -height/2, 0), (0, height/2, 0)]
    for i in range(segments):
        angle = 2 * math.pi * i / segments
        x = radius * math.cos(angle)
        z = radius * math.sin(angle)
        verts.append((x, -height/2, z))
    for i in range(segments):
        angle = 2 * math.pi * i / segments
        x = radius * math.cos(angle)
        z = radius * math.sin(angle)
        verts.append((x, height/2, z))
    
    faces = []
    for i in range(segments):
        n = 2 + i
        nn = 2 + ((i + 1) % segments)
        faces.append((0, nn, n))
        faces.append((1, n, nn))
        faces.append((n, nn, n + segments))
        faces.append((nn, nn + segments, n + segments))
    return verts, faces

def generate_assault_rifle():
    """M4A1-style rifle"""
    verts = []
    faces = []
    
    # Barrel
    v, f = cylinder(0.03, 1.5, 8)
    offset = len(verts)
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    # Body
    v, f = box(0.12, 0.5, 0.08)
    offset = len(verts)
    v = [(x, y + 0.35, z) for x, y, z in v]
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    # Stock
    v, f = box(0.06, 0.3, 0.06)
    offset = len(verts)
    v = [(x, y - 0.55, z) for x, y, z in v]
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    # Magazine
    v, f = box(0.04, 0.25, 0.06)
    offset = len(verts)
    v = [(x, y + 0.2, z - 0.08) for x, y, z in v]
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    # Handle
    v, f = box(0.03, 0.15, 0.04)
    offset = len(verts)
    v = [(x, y + 0.3, z - 0.05) for x, y, z in v]
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    return verts, faces

def generate_pistol():
    """Pistol shape"""
    verts = []
    faces = []
    
    # Barrel
    v, f = box(0.03, 0.3, 0.04)
    offset = len(verts)
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    # Body
    v, f = box(0.05, 0.2, 0.06)
    offset = len(verts)
    v = [(x, y + 0.15, z) for x, y, z in v]
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    # Magazine
    v, f = box(0.03, 0.15, 0.04)
    offset = len(verts)
    v = [(x, y + 0.1, z - 0.04) for x, y, z in v]
    verts.extend(v)
    faces.extend([tuple(offset + x for x in face) for face in f])
    
    return verts, faces

def generate_prop_crate():
    """Ammo crate / box prop"""
    return box(0.5, 0.5, 0.5)

def generate_prop_barrel():
    """Oil drum"""
    return cylinder(0.3, 0.6, 12)

def generate_all_models():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    models = {
        'AssaultRifle.obj': generate_assault_rifle(),
        'Pistol.obj': generate_pistol(),
        'Prop_Crate.obj': generate_prop_crate(),
        'Prop_Barrel.obj': generate_prop_barrel(),
    }
    
    for name, (verts, faces) in models.items():
        filepath = os.path.join(OUTPUT_DIR, name)
        write_obj(filepath, verts, faces)
    
    print(f"\n✓ {len(models)} 3D models generated in {OUTPUT_DIR}")

if __name__ == '__main__':
    generate_all_models()
