#include "ZeroDawnKillcamManager.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../UI/ZeroDawnHUD.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

UZeroDawnKillcamManager::UZeroDawnKillcamManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UZeroDawnKillcamManager::StartKillcam(AActor* KillerActor, APawn* DeadPawn)
{
	if (!KillerActor || !DeadPawn)
	{
		return;
	}

	// Store a weak reference to the dead pawn for safe restoration later
	DeadPawnPtr = DeadPawn;

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC || !PC->IsLocalController())
	{
		return;
	}

	// Disable input during killcam
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);

	// Hide the HUD during killcam
	AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(PC->GetHUD());
	if (ZDHUD)
	{
		ZDHUD->HideHUDWidget();
	}

	// Transition camera to the killer's viewpoint with a smooth blend
	PC->SetViewTargetWithBlend(KillerActor, ViewBlendTime, VTBlend_Cubic, 0.0f, false);

	// Set a timer for the killcam duration — EndKillcam fires when time is up
	GetWorld()->GetTimerManager().SetTimer(KillcamTimerHandle, this, &UZeroDawnKillcamManager::EndKillcam, KillcamDuration, false);
}

void UZeroDawnKillcamManager::EndKillcam()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		return;
	}

	// Restore the view target to the dead pawn (or a safe fallback)
	APawn* DeadPawn = DeadPawnPtr.Get();
	if (DeadPawn && IsValid(DeadPawn))
	{
		PC->SetViewTargetWithBlend(DeadPawn, ViewBlendTime, VTBlend_Cubic, 0.0f, false);
	}
	else if (PC->GetPawn() && IsValid(PC->GetPawn()))
	{
		// Fallback: use the controller's current pawn if available
		PC->SetViewTargetWithBlend(PC->GetPawn(), ViewBlendTime, VTBlend_Cubic, 0.0f, false);
	}
	else
	{
		// Fallback: view the controller itself
		PC->SetViewTargetWithBlend(PC, ViewBlendTime, VTBlend_Cubic, 0.0f, false);
	}

	// Re-enable input
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);

	// Restore HUD visibility
	AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(PC->GetHUD());
	if (ZDHUD)
	{
		ZDHUD->ShowHUDWidget();
	}

	// Trigger the respawn sequence via the dead character
	if (DeadPawn && IsValid(DeadPawn))
	{
		AZeroDawnCharacter* DeadCharacter = Cast<AZeroDawnCharacter>(DeadPawn);
		if (DeadCharacter)
		{
			DeadCharacter->RequestRespawn();
		}
	}
}
