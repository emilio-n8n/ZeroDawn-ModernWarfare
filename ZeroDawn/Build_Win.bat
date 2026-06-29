@echo off
REM Build script for Zero Dawn: Modern Warfare - Windows
REM Usage: Build_Win.bat [Development|Shipping]

set BUILD_CONFIG=%1
if "%BUILD_CONFIG%"=="" set BUILD_CONFIG=Development

set UE_ENGINE_PATH=%UE_ENGINE_PATH%
if "%UE_ENGINE_PATH%"=="" set UE_ENGINE_PATH=C:\Program Files\Epic Games\UE_5.7

set PROJECT_PATH=%~dp0

echo === Building Zero Dawn: Modern Warfare ===
echo Config: %BUILD_CONFIG%
echo Engine: %UE_ENGINE_PATH%
echo Project: %PROJECT_PATH%

"%UE_ENGINE_PATH%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun ^
    -project="%PROJECT_PATH%ZeroDawn.uproject" ^
    -noP4 ^
    -platform=Win64 ^
    -clientconfig=%BUILD_CONFIG% ^
    -serverconfig=%BUILD_CONFIG% ^
    -cook ^
    -allmaps ^
    -build ^
    -stage ^
    -pak ^
    -archive ^
    -archivedirectory="%PROJECT_PATH%Archives\Win64"

echo === Build complete! ===
echo Output: %PROJECT_PATH%Archives\Win64
pause
