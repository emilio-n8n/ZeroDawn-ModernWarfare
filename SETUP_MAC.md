# Setup sur Mac

## 1. Installer Unreal Engine 5.7+

1. Télécharge l'Epic Games Launcher depuis https://store.epicgames.com/
2. Connecte-toi avec ton compte Epic
3. Va dans l'onglet "Unreal Engine" → "Library"
4. Clique sur "+" → "Install Engine Version" → choisis 5.7.4
5. Attends le téléchargement (~40 Go)

## 2. Préparer le projet

```bash
# Option A: Copier les fichiers sur ton Mac
scp -r user@vps:/teamspace/studios/this_studio/AAA-game/ZeroDawn ~/Desktop/

# Option B: Git
git clone <ton-repo> ZeroDawn
```

## 3. Générer les fichiers Xcode

1. Clic droit sur `ZeroDawn.uproject` → "Generate Xcode project" (ou "Service" → "Generate Xcode Project")
2. Ça va créer `ZeroDawn.xcworkspace`

## 4. Compiler et lancer

1. Ouvre `ZeroDawn.xcworkspace`
2. Choisis "Development Editor" comme scheme
3. Cmd+B pour compiler (première fois ~15-30 min)
4. Cmd+R pour lancer l'éditeur

## 5. Tester

Une fois dans l'éditeur :
- Ouvre le niveau `Lvl_Shooter` dans Content/FirstPerson
- Clique sur "Play" pour tester le FPS
- Les armes : clic gauche pour tirer, R pour recharger, clic droit pour viser

## Build standalone

Dans l'éditeur : File → Package Project → Windows/Mac
