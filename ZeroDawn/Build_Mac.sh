#!/bin/bash
# Build script for Zero Dawn: Modern Warfare - Mac
# Usage: ./Build_Mac.sh [Development|Shipping]

set -e

BUILD_CONFIG="${1:-Development}"
UE_ENGINE_PATH="${UE_ENGINE_PATH:-/Users/Shared/EpicGames/UE_5.7}"
PROJECT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== Building Zero Dawn: Modern Warfare ==="
echo "Config: $BUILD_CONFIG"
echo "Engine: $UE_ENGINE_PATH"
echo "Project: $PROJECT_PATH"

"$UE_ENGINE_PATH/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun \
    -project="$PROJECT_PATH/ZeroDawn.uproject" \
    -noP4 \
    -platform=Mac \
    -clientconfig="$BUILD_CONFIG" \
    -serverconfig="$BUILD_CONFIG" \
    -cook \
    -allmaps \
    -build \
    -stage \
    -pak \
    -archive \
    -archivedirectory="$PROJECT_PATH/Archives/Mac"

echo "=== Build complete! ==="
echo "Output: $PROJECT_PATH/Archives/Mac"
