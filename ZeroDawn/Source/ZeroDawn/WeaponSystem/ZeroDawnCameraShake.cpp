#include "ZeroDawnCameraShake.h"

UZeroDawnCameraShake::UZeroDawnCameraShake(const FObjectInitializer& OI) : Super(OI) {}
UZeroDawnExplosionShake::UZeroDawnExplosionShake(const FObjectInitializer& OI) : Super(OI) {}
UZeroDawnHitShake::UZeroDawnHitShake(const FObjectInitializer& OI) : Super(OI) {}

void UZeroDawnCameraShake::PlayExplosionShake(APlayerController* PC)
{
	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraShake(UZeroDawnExplosionShake::StaticClass(), 1.0f);
	}
}

void UZeroDawnCameraShake::PlayKillstreakShake(APlayerController* PC)
{
	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraShake(UZeroDawnExplosionShake::StaticClass(), 0.5f);
	}
}

void UZeroDawnCameraShake::PlayHitShake(APlayerController* PC, float Intensity)
{
	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraShake(UZeroDawnHitShake::StaticClass(), Intensity);
	}
}
