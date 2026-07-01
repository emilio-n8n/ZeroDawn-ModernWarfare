#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "ZeroDawnHUDWidget.generated.h"

UCLASS()
class UZeroDawnHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Update the health bar progress. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	/** Update the armor bar progress. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateArmorBar(float CurrentArmor, float MaxArmor);

	/** Update the ammo counter display (current, reserve, max magazine). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmoDisplay(int32 CurrentAmmo, int32 ReserveAmmo, int32 MaxMagazine);

	/** Update the weapon name text. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWeaponName(const FString& WeaponName);

	/** Update the killstreak counter display. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateKillstreakDisplay(int32 CurrentKillstreak);

	/** Show a kill notification with killer, victim and weapon icon. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowKillNotification(const FString& KillerName, const FString& VictimName, const FString& WeaponIcon);

	/** Update the score display (team vs enemy). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScoreDisplay(int32 TeamScore, int32 EnemyScore);

	/** Update the match timer text. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateMatchTimer(const FString& TimeString);

	/** Show the low-health warning overlay. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowLowHealthWarning();

	/** Hide the low-health warning overlay. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideLowHealthWarning();

protected:
	/** Bound UMG Widgets — must share names with the UMG blueprint widgets. */

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ArmorBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReserveAmmoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillstreakText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillNotificationText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MatchTimerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> LowHealthOverlay;
};
