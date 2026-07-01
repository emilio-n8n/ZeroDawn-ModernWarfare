#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnPlayerController.generated.h"

class UZeroDawnKillcamManager;
class UZeroDawnSpectatorOverlay;
class UUserWidget;
class AZeroDawnCharacter;

/**
 * Custom PlayerController for Zero Dawn.
 * Hosts the KillcamManager component, spectator mode, and per-controller systems.
 */
UCLASS()
class AZeroDawnPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZeroDawnPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Killcam manager component — handles death-cam view switching and respawn timing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Killcam")
	UZeroDawnKillcamManager* KillcamManager;

	// ============================================================
	// Spectator Mode
	// ============================================================

	/** Whether this controller is currently in spectator mode */
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_bIsSpectating, VisibleAnywhere, BlueprintReadOnly, Category = "Spectator")
	bool bIsSpectating = false;

	/** Called when bIsSpectating changes on the client — handles overlay cleanup */
	UFUNCTION()
	void OnRep_bIsSpectating();

	/** The class to use for the spectator overlay widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectator")
	TSubclassOf<UUserWidget> SpectatorOverlayClass;

	/** The spectator overlay widget instance (spawned on the local client only) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spectator")
	TObjectPtr<UZeroDawnSpectatorOverlay> SpectatorOverlay;

	/**
	 * Enter spectator mode: finds alive teammates, sets view to first one,
	 * shows the spectator overlay widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void EnterSpectatorMode();

	/**
	 * Exit spectator mode: restores normal HUD, hides overlay, allows respawn.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void ExitSpectatorMode();

	/**
	 * Cycle to the next alive teammate in the spectator target list.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void CycleNextTeammate();

	/**
	 * Cycle to the previous alive teammate in the spectator target list.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void CyclePrevTeammate();

	/**
	 * Refresh the list of alive teammates available for spectating.
	 * Should be called when a teammate dies or respawns.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void RefreshSpectateTargets();

	/**
	 * Update the spectator overlay with the currently observed player's info.
	 * Called every Tick while spectating.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void UpdateSpectatorOverlay();

protected:
	/** Cached list of alive teammates to spectate */
	TArray<TWeakObjectPtr<AZeroDawnCharacter>> SpectateTargets;

	/** Index into SpectateTargets for the currently observed player */
	int32 CurrentSpectateTargetIndex = 0;

	/**
	 * Internal: set the view target to the spectate target at the given index.
	 * Updates CurrentSpectateTargetIndex and refreshes the overlay.
	 */
	void UpdateSpectateViewTarget(int32 Index);
};
