#include "ZeroDawnHUDWidget.h"

void UZeroDawnHUDWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBar && MaxHealth > 0.0f)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UZeroDawnHUDWidget::UpdateArmorBar(float CurrentArmor, float MaxArmor)
{
	if (ArmorBar && MaxArmor > 0.0f)
	{
		ArmorBar->SetPercent(CurrentArmor / MaxArmor);
	}
}

void UZeroDawnHUDWidget::UpdateAmmoDisplay(int32 CurrentAmmo, int32 ReserveAmmo, int32 MaxMagazine)
{
	if (AmmoCountText)
	{
		AmmoCountText->SetText(FText::AsNumber(CurrentAmmo));
	}
	if (ReserveAmmoText)
	{
		ReserveAmmoText->SetText(FText::AsNumber(ReserveAmmo));
	}
}

void UZeroDawnHUDWidget::UpdateWeaponName(const FString& WeaponName)
{
	if (WeaponNameText)
	{
		WeaponNameText->SetText(FText::FromString(WeaponName));
	}
}

void UZeroDawnHUDWidget::UpdateKillstreakDisplay(int32 CurrentKillstreak)
{
	if (KillstreakText)
	{
		if (CurrentKillstreak > 0)
		{
			KillstreakText->SetText(FText::Format(
				NSLOCTEXT("HUD", "KillstreakFormat", "{0}x Killstreak"),
				FText::AsNumber(CurrentKillstreak)
			));
			KillstreakText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			KillstreakText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UZeroDawnHUDWidget::ShowKillNotification(const FString& KillerName, const FString& VictimName, const FString& WeaponIcon)
{
	if (KillNotificationText)
	{
		KillNotificationText->SetText(FText::Format(
			NSLOCTEXT("HUD", "KillNotificationFormat", "{0} \u2192 {1} [{2}]"),
			FText::FromString(KillerName),
			FText::FromString(VictimName),
			FText::FromString(WeaponIcon)
		));
		KillNotificationText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZeroDawnHUDWidget::UpdateScoreDisplay(int32 TeamScore, int32 EnemyScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::Format(
			NSLOCTEXT("HUD", "ScoreFormat", "{0} - {1}"),
			FText::AsNumber(TeamScore),
			FText::AsNumber(EnemyScore)
		));
	}
}

void UZeroDawnHUDWidget::UpdateMatchTimer(const FString& TimeString)
{
	if (MatchTimerText)
	{
		MatchTimerText->SetText(FText::FromString(TimeString));
	}
}

void UZeroDawnHUDWidget::ShowLowHealthWarning()
{
	if (LowHealthOverlay)
	{
		LowHealthOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZeroDawnHUDWidget::HideLowHealthWarning()
{
	if (LowHealthOverlay)
	{
		LowHealthOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
