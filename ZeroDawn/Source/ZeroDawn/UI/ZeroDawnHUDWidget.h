#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "Blueprint/UserWidget.h"
#include "ZeroDawnHUDWidget.generated.h"

UCLASS()
class UZeroDawnHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateArmorBar(float CurrentArmor, float MaxArmor);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateAmmoDisplay(int32 CurrentAmmo, int32 ReserveAmmo, int32 MaxMagazine);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateWeaponName(const FString& WeaponName);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateKillstreakDisplay(int32 CurrentKillstreak);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowKillNotification(const FString& KillerName, const FString& VictimName, const FString& WeaponIcon);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateScoreDisplay(int32 TeamScore, int32 EnemyScore);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateMatchTimer(const FString& TimeString);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowLowHealthWarning();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void HideLowHealthWarning();
};
