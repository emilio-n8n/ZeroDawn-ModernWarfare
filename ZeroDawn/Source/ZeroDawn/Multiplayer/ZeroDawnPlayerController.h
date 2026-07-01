#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnPlayerController.generated.h"

class UZeroDawnKillcamManager;

/**
 * Custom PlayerController for Zero Dawn.
 * Hosts the KillcamManager component and any future per-controller systems.
 */
UCLASS()
class AZeroDawnPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZeroDawnPlayerController();

	/** Killcam manager component — handles death-cam view switching and respawn timing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Killcam")
	UZeroDawnKillcamManager* KillcamManager;
};
