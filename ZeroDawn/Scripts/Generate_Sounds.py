#!/usr/bin/env python3
"""Generate game audio assets using synthesis (no external AI needed)"""

import numpy as np
import struct
import os
import math

SAMPLE_RATE = 44100

def write_wav(filename, samples):
    """Write mono 16-bit WAV file"""
    samples = np.clip(samples, -1.0, 1.0)
    samples_int = (samples * 32767).astype(np.int16)
    
    with open(filename, 'wb') as f:
        data_size = len(samples_int) * 2
        # RIFF header
        f.write(b'RIFF')
        f.write(struct.pack('<I', 36 + data_size))
        f.write(b'WAVE')
        # fmt chunk
        f.write(b'fmt ')
        f.write(struct.pack('<I', 16))  # chunk size
        f.write(struct.pack('<H', 1))   # PCM
        f.write(struct.pack('<H', 1))   # mono
        f.write(struct.pack('<I', SAMPLE_RATE))
        f.write(struct.pack('<I', SAMPLE_RATE * 2))  # byte rate
        f.write(struct.pack('<H', 2))   # block align
        f.write(struct.pack('<H', 16))  # bits per sample
        # data chunk
        f.write(b'data')
        f.write(struct.pack('<I', data_size))
        f.write(samples_int.tobytes())

def envelope(samples, attack=0.01, release=0.1):
    """Apply attack/release envelope"""
    n = len(samples)
    a = int(attack * SAMPLE_RATE)
    r = int(release * SAMPLE_RATE)
    env = np.ones(n)
    env[:a] = np.linspace(0, 1, a)
    env[-r:] = np.linspace(1, 0, r)
    return samples * env

def generate_gunshot():
    """Realistic gunshot sound"""
    duration = 0.3
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    # Main crack - short noise burst
    noise = np.random.normal(0, 1, len(t))
    crack = np.exp(-t * 80) * noise
    
    # Low frequency boom
    boom = np.exp(-t * 15) * np.sin(2 * np.pi * 80 * t) * 0.6
    
    # Mid frequency punch
    punch = np.exp(-t * 40) * np.sin(2 * np.pi * 400 * t) * 0.3
    
    # High frequency snap
    snap = np.exp(-t * 100) * np.sin(2 * np.pi * 2000 * t) * 0.15
    
    sound = crack * 0.5 + boom * 0.3 + punch * 0.15 + snap * 0.05
    sound = envelope(sound, 0.001, 0.2)
    return sound * 0.8

def generate_pistol_shot():
    """Pistol shot - sharper, less boom"""
    duration = 0.2
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    noise = np.random.normal(0, 1, len(t))
    crack = np.exp(-t * 120) * noise
    snap = np.exp(-t * 150) * np.sin(2 * np.pi * 3000 * t) * 0.2
    punch = np.exp(-t * 60) * np.sin(2 * np.pi * 600 * t) * 0.25
    
    sound = crack * 0.6 + punch * 0.2 + snap * 0.2
    sound = envelope(sound, 0.001, 0.15)
    return sound * 0.7

def generate_shotgun():
    """Shotgun blast"""
    duration = 0.4
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    noise = np.random.normal(0, 1, len(t))
    crack = np.exp(-t * 60) * noise
    boom = np.exp(-t * 10) * np.sin(2 * np.pi * 60 * t) * 0.8
    
    sound = crack * 0.4 + boom * 0.6
    sound = envelope(sound, 0.002, 0.3)
    return sound * 0.9

def generate_footstep():
    """Footstep on concrete"""
    duration = 0.15
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    # Impact
    thud = np.exp(-t * 60) * np.sin(2 * np.pi * 120 * t) * 0.5
    scrape = np.exp(-t * 30) * np.random.normal(0, 0.3, len(t))
    
    sound = thud + scrape
    sound = envelope(sound, 0.002, 0.1)
    return sound * 0.6

def generate_reload():
    """Reload sound - click and slide"""
    duration = 0.5
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    # Magazine release click
    click1 = np.exp(-t * 100) * np.sin(2 * np.pi * 5000 * t) * 0.3
    click1_pos = int(0.05 * SAMPLE_RATE)
    click1 = np.roll(click1, click1_pos)
    click1[:click1_pos] = 0
    
    # Magazine insert click
    click2 = np.exp(-t * 80) * np.sin(2 * np.pi * 4000 * t) * 0.3
    click2_pos = int(0.3 * SAMPLE_RATE)
    click2 = np.roll(click2, click2_pos)
    click2[:click2_pos] = 0
    
    # Slide release
    slide = np.exp(-t * 40) * np.sin(2 * np.pi * 800 * t) * 0.2
    slide_pos = int(0.35 * SAMPLE_RATE)
    slide = np.roll(slide, slide_pos)
    slide[:slide_pos] = 0
    
    sound = click1 + click2 + slide
    return envelope(sound, 0.001, 0.1) * 0.7

def generate_explosion():
    """Explosion sound"""
    duration = 1.5
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    noise = np.random.normal(0, 1, len(t))
    rumble = np.exp(-t * 5) * noise * 0.5
    boom = np.exp(-t * 8) * np.sin(2 * np.pi * 50 * t) * 0.8
    crack = np.exp(-t * 40) * np.random.normal(0, 1, len(t)) * 0.3
    
    sound = rumble + boom + crack
    sound = envelope(sound, 0.01, 0.5)
    return sound * 0.9

def generate_combat_music():
    """Intense combat music loop (10 seconds)"""
    duration = 10.0
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    # Bass drone
    bass = np.sin(2 * np.pi * 55 * t) * 0.3
    bass += np.sin(2 * np.pi * 82.5 * t) * 0.2  # Fifth
    
    # Percussion hits
    beat = np.zeros_like(t)
    beat_interval = 0.5
    for i in range(int(duration / beat_interval)):
        pos = int(i * beat_interval * SAMPLE_RATE)
        if pos < len(t):
            hit = np.exp(-np.linspace(0, 0.15, int(0.15 * SAMPLE_RATE)) * 30)
            hit_len = min(len(hit), len(t) - pos)
            beat[pos:pos+hit_len] += hit[:hit_len] * 0.5
    
    # Tension layer
    tension = np.sin(2 * np.pi * 440 * t + np.sin(2 * np.pi * 0.5 * t) * 3) * 0.1
    
    sound = bass + beat + tension
    sound = sound * 0.7
    return sound

def generate_hit_sound():
    """Bullet hit impact"""
    duration = 0.08
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    thwack = np.exp(-t * 80) * np.random.normal(0, 1, len(t)) * 0.5
    thump = np.exp(-t * 50) * np.sin(2 * np.pi * 200 * t) * 0.4
    
    sound = thwack + thump
    sound = envelope(sound, 0.001, 0.05)
    return sound * 0.7

def generate_empty_mag():
    """Empty magazine click"""
    duration = 0.1
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    click = np.exp(-t * 100) * np.sin(2 * np.pi * 6000 * t) * 0.5
    return envelope(click, 0.001, 0.05)

def generate_menu_click():
    """UI menu click sound"""
    duration = 0.08
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration))
    
    beep = np.sin(2 * np.pi * 800 * t) * 0.3
    return envelope(beep, 0.002, 0.06)

def save_all_sounds(output_dir):
    """Generate and save all game sounds"""
    os.makedirs(output_dir, exist_ok=True)
    
    sounds = {
        'Gunshot_Rifle.wav': generate_gunshot(),
        'Gunshot_Pistol.wav': generate_pistol_shot(),
        'Gunshot_Shotgun.wav': generate_shotgun(),
        'Footstep.wav': generate_footstep(),
        'Reload.wav': generate_reload(),
        'Explosion.wav': generate_explosion(),
        'Hit_Impact.wav': generate_hit_sound(),
        'Empty_Mag.wav': generate_empty_mag(),
        'Menu_Click.wav': generate_menu_click(),
        'Combat_Music_Loop.wav': generate_combat_music(),
    }
    
    for name, samples in sounds.items():
        filepath = os.path.join(output_dir, name)
        write_wav(filepath, samples)
        print(f"  ✓ {name} ({os.path.getsize(filepath)/1024:.1f} KB)")
    
    print(f"\n✓ {len(sounds)} audio files generated in {output_dir}")

if __name__ == '__main__':
    import sys
    output = sys.argv[1] if len(sys.argv) > 1 else '/teamspace/studios/this_studio/AAA-game/ZeroDawn/Content/ZeroDawn/Audio'
    save_all_sounds(output)
