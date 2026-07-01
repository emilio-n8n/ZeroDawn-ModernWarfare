#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/ScrollBox.h"
#include "ZeroDawnLobbyWidget.generated.h"

class UZeroDawnSessionSubsystem;
class AZeroDawnPlayerState;

/**
 * UZeroDawnLobbyWidget
 *
 * Lobby user widget displayed after hosting or joining a session.
 * Shows the player list with per-player ready state, a ready toggle,
 * a game mode selector (host only), and a Start Game button (host only).
 */
UCLASS()
class UZeroDawnLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// ============================================================
	// Bound Widgets (from UMG blueprint)
	// ============================================================

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyToggleButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReadyButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> GameModeSelector;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> PlayerListBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatusText;

	// ============================================================
	// Button Handlers
	// ============================================================

	UFUNCTION()
	void OnReadyToggleClicked();

	UFUNCTION()
	void OnGameModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnStartGameClicked();

	// ============================================================
	// Helpers
	// ============================================================

	/** Refresh the displayed player list from the current GameState player array. */
	void RefreshPlayerList();

	/** Set the status text at the bottom of the lobby. */
	void SetStatus(const FString& Message);

	/** Build the display string for an EGameModeType value. */
	FString GameModeToString(EGameModeType Mode) const;

private:
	/** Whether the local player is currently marked as ready. */
	bool bLocalReady = false;

	/** The game mode selected via the combo box. */
	EGameModeType SelectedGameMode = EGameModeType::TeamDeathmatch;

	/** Cached pointer to the session subsystem. */
	TWeakObjectPtr<UZeroDawnSessionSubsystem> SessionSubsystem;

	/** Throttle the player list refresh to avoid per-frame cost. */
	float RefreshTimer = 0.0f;
	static constexpr float RefreshInterval = 1.0f;

	/** Map of game mode enum values to their display strings for the combo box. */
	TMap<FString, EGameModeType> GameModeMap;
};
