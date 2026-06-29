#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnHitmarker.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UZeroDawnHitmarker : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnHitmarker();

	UFUNCTION(BlueprintImplementableEvent, Category = "Hitmarker")
	void ShowHitmarker(bool bHeadshot);

	UFUNCTION(BlueprintImplementableEvent, Category = "Hitmarker")
	void ShowKillConfirmation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHitmarker(bool bHeadshot);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastKillConfirmation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	USoundCue* HitmarkerSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	USoundCue* HeadshotSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	USoundCue* KillSound;
};
