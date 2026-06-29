#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnKillstreakHUD.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UZeroDawnKillstreakHUD : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnKillstreakHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreak")
	void ShowKillstreakNotification(EKillstreakType Type, const FString& StreakName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreak")
	void ShowKillstreakProgress(int32 CurrentKills, int32 NextStreakKills);

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreak")
	void ShowStreakCounter(int32 CurrentStreak);

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreak")
	void PlayKillstreakSound(EKillstreakType Type);

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreak")
	void ShowNukeWarning(int32 SecondsUntilNuke);

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreak")
	void OnNukeDetonated();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowStreak(int32 CurrentKillstreak);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastNukeDetonated();

	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	FString GetStreakName(EKillstreakType Type) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* StreakCounterSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* NukeWarningSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* NukeDetonationSound;
};
