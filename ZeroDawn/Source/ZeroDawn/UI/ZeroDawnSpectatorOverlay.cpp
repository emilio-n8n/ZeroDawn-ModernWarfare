#include "ZeroDawnSpectatorOverlay.h"

void UZeroDawnSpectatorOverlay::UpdateObservedInfo(const FString& PlayerName, float CurrentHealth, float MaxHealth)
{
	if (ObservedNameText)
	{
		ObservedNameText->SetText(FText::FromString(PlayerName));
	}

	if (ObservedHealthText && MaxHealth > 0.0f)
	{
		FString HealthStr = FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth);
		ObservedHealthText->SetText(FText::FromString(HealthStr));
	}

	if (ObservedHealthBar && MaxHealth > 0.0f)
	{
		float Percent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
		ObservedHealthBar->SetPercent(Percent);

		// Tint the health bar based on health percentage
		FLinearColor BarColor;
		if (Percent > 0.5f)
			BarColor = FLinearColor::Green;
		else if (Percent > 0.25f)
			BarColor = FLinearColor::Yellow;
		else
			BarColor = FLinearColor::Red;

		ObservedHealthBar->SetFillColorAndOpacity(BarColor);
	}
}

void UZeroDawnSpectatorOverlay::ShowOverlay()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UZeroDawnSpectatorOverlay::HideOverlay()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (ObservedNameText)
	{
		ObservedNameText->SetText(FText::GetEmpty());
	}
	if (ObservedHealthText)
	{
		ObservedHealthText->SetText(FText::GetEmpty());
	}
	if (ObservedHealthBar)
	{
		ObservedHealthBar->SetPercent(0.0f);
	}
}
