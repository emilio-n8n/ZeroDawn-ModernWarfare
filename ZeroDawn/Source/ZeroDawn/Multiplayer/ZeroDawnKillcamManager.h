#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnKillcamManager.generated.h"

/**
 * Killcam Manager Component
 * 
 * Attached to the PlayerController. On player death, switches the camera view to the
 * killer's viewpoint for a configurable duration (default 3 seconds), then restores
 * the view, re-enables input/HUD, and triggers the respawn sequence.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UZeroDawnKillcamManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnKillcamManager();

	/**
	 * Start the killcam: attach camera to the killer's viewpoint for KillcamDuration seconds.
	 * @param KillerActor - The actor that killed the player (viewtarget during killcam)
	 * @param DeadPawn   - The dead player's pawn (viewtarget restored after killcam)
	 */
	void StartKillcam(AActor* KillerActor, APawn* DeadPawn);

	/**
	 * End the killcam: restore view to the dead pawn, re-enable input/HUD, trigger respawn.
	 */
	UFUNCTION()
	void EndKillcam();

protected:
	/** Duration of the killcam in seconds (default 3 seconds per design spec) */
	UPROPERTY(EditDefaultsOnly, Category = "Killcam")
	float KillcamDuration = 3.0f;

	/** Blend time when transitioning camera to/from killcam view */
	UPROPERTY(EditDefaultsOnly, Category = "Killcam")
	float ViewBlendTime = 0.5f;

	/** Weak pointer to the dead pawn to safely restore view after killcam */
	TWeakObjectPtr<APawn> DeadPawnPtr;

	/** Timer handle for the killcam duration (must be a member, not local) */
	FTimerHandle KillcamTimerHandle;
};
