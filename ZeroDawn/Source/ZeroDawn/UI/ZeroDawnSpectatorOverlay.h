#pragma once
#include "../ZeroDawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ZeroDawnSpectatorOverlay.generated.h"

/**
 * Spectator overlay widget displayed when the player is spectating a teammate.
 * Shows the observed player's name and current health.
 */
UCLASS()
class UZeroDawnSpectatorOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Update the overlay with info about the currently observed player.
	 * @param PlayerName - Display name of the observed player
	 * @param CurrentHealth - Current health value of the observed player
	 * @param MaxHealth - Maximum health value of the observed player
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void UpdateObservedInfo(const FString& PlayerName, float CurrentHealth, float MaxHealth);

	/**
	 * Show the spectator overlay.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void ShowOverlay();

	/**
	 * Hide the spectator overlay.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void HideOverlay();

protected:
	/** Text block showing the observed player's name with a prefix label */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ObservedNameText;

	/** Text block showing the observed player's health as "HP / MaxHP" */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ObservedHealthText;

	/** Progress bar showing the observed player's remaining health */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ObservedHealthBar;

	/** Label prefix before the player name (e.g., "Spectating: ") */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SpectatorLabel;
};
