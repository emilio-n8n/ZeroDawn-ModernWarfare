#include "ZeroDawnPlayerController.h"
#include "ZeroDawnKillcamManager.h"

AZeroDawnPlayerController::AZeroDawnPlayerController()
{
	// Create the killcam manager as a default subobject on the PlayerController
	KillcamManager = CreateDefaultSubobject<UZeroDawnKillcamManager>(TEXT("KillcamManager"));
}
