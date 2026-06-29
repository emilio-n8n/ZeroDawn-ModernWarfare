#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnCameraShake.generated.h"

UCLASS()
class UZeroDawnCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()

public:
	UZeroDawnCameraShake(const FObjectInitializer& OI);

	static void PlayExplosionShake(APlayerController* PC);
	static void PlayKillstreakShake(APlayerController* PC);
	static void PlayHitShake(APlayerController* PC, float Intensity = 1.0f);
};

UCLASS()
class UZeroDawnExplosionShake : public UCameraShakeBase
{
	GENERATED_BODY()
public:
	UZeroDawnExplosionShake(const FObjectInitializer& OI);
};

UCLASS()
class UZeroDawnHitShake : public UCameraShakeBase
{
	GENERATED_BODY()
public:
	UZeroDawnHitShake(const FObjectInitializer& OI);
};
