#include "ZeroDawnPlayerController.h"
#include "ZeroDawnKillcamManager.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../UI/ZeroDawnSpectatorOverlay.h"
#include "../UI/ZeroDawnHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

AZeroDawnPlayerController::AZeroDawnPlayerController()
{
	// Create the killcam manager as a default subobject on the PlayerController
	KillcamManager = CreateDefaultSubobject<UZeroDawnKillcamManager>(TEXT("KillcamManager"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void AZeroDawnPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Cache the spectator overlay class from HUD defaults if not already set
	if (!SpectatorOverlayClass)
	{
		AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(GetHUD());
		if (ZDHUD)
		{
			SpectatorOverlayClass = ZDHUD->SpectatorOverlayClass;
		}
	}
}

void AZeroDawnPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWorld())
	{
		return;
	}

	// Periodically update the spectator overlay while in spectator mode
	if (bIsSpectating && IsLocalController())
	{
		UpdateSpectatorOverlay();
	}
}

void AZeroDawnPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AZeroDawnPlayerController, bIsSpectating, COND_OwnerOnly);
}

void AZeroDawnPlayerController::OnRep_bIsSpectating()
{
	// When bIsSpectating changes on the client, handle visual cleanup
	// (called automatically by the engine when the replicated value updates)
	if (!bIsSpectating && IsLocalController())
	{
		// Remove spectator overlay if it exists
		if (SpectatorOverlay)
		{
			SpectatorOverlay->RemoveFromParent();
			SpectatorOverlay = nullptr;
		}

		// Restore HUD visibility
		AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(GetHUD());
		if (ZDHUD)
		{
			ZDHUD->ShowHUDWidget();
		}

		// Re-enable input
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);
	}
}

void AZeroDawnPlayerController::EnterSpectatorMode()
{
	if (bIsSpectating)
	{
		return;
	}

	bIsSpectating = true;
	CurrentSpectateTargetIndex = 0;

	// Disable movement input during spectator mode
	SetIgnoreMoveInput(true);
	// Allow looking around in third-person spectator view
	SetIgnoreLookInput(false);

	// Cache the spectator overlay class from HUD if not set
	if (!SpectatorOverlayClass)
	{
		AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(GetHUD());
		if (ZDHUD)
		{
			SpectatorOverlayClass = ZDHUD->SpectatorOverlayClass;
		}
	}

	// Client-only: spawn the spectator overlay widget
	if (IsLocalController())
	{
		if (SpectatorOverlayClass && !SpectatorOverlay)
		{
			SpectatorOverlay = CreateWidget<UZeroDawnSpectatorOverlay>(this, SpectatorOverlayClass);
			if (SpectatorOverlay)
			{
				SpectatorOverlay->AddToViewport(200); // High Z-order to appear above HUD
				SpectatorOverlay->ShowOverlay();
			}
		}

		// Hide the main HUD during spectator mode
		AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(GetHUD());
		if (ZDHUD)
		{
			ZDHUD->HideHUDWidget();
		}
	}

	// Find all alive teammates and set view to the first one
	RefreshSpectateTargets();

	if (SpectateTargets.Num() > 0)
	{
		UpdateSpectateViewTarget(0);
	}

	UE_LOG(LogTemp, Log, TEXT("Spectator: Entered spectator mode with %d alive teammates."), SpectateTargets.Num());
}

void AZeroDawnPlayerController::ExitSpectatorMode()
{
	if (!bIsSpectating)
	{
		return;
	}

	bIsSpectating = false;
	CurrentSpectateTargetIndex = 0;
	SpectateTargets.Empty();

	// Re-enable movement input
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	// Client-only: remove the spectator overlay and restore HUD
	if (IsLocalController())
	{
		if (SpectatorOverlay)
		{
			SpectatorOverlay->RemoveFromParent();
			SpectatorOverlay = nullptr;
		}

		AZeroDawnHUD* ZDHUD = Cast<AZeroDawnHUD>(GetHUD());
		if (ZDHUD)
		{
			ZDHUD->ShowHUDWidget();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Spectator: Exited spectator mode."));
}

void AZeroDawnPlayerController::CycleNextTeammate()
{
	if (!bIsSpectating)
	{
		return;
	}

	// Refresh the list in case teammates have died or respawned
	RefreshSpectateTargets();

	if (SpectateTargets.Num() <= 0)
	{
		return;
	}

	int32 NewIndex = CurrentSpectateTargetIndex + 1;
	if (NewIndex >= SpectateTargets.Num())
	{
		NewIndex = 0; // Wrap around to first
	}

	UpdateSpectateViewTarget(NewIndex);
}

void AZeroDawnPlayerController::CyclePrevTeammate()
{
	if (!bIsSpectating)
	{
		return;
	}

	// Refresh the list in case teammates have died or respawned
	RefreshSpectateTargets();

	if (SpectateTargets.Num() <= 0)
	{
		return;
	}

	int32 NewIndex = CurrentSpectateTargetIndex - 1;
	if (NewIndex < 0)
	{
		NewIndex = SpectateTargets.Num() - 1; // Wrap around to last
	}

	UpdateSpectateViewTarget(NewIndex);
}

void AZeroDawnPlayerController::RefreshSpectateTargets()
{
	if (!HasAuthority() && !IsLocalController())
	{
		return; // Only run on server or owning client
	}

	SpectateTargets.Empty();

	// Determine the spectator's own team
	AZeroDawnPlayerState* ZDPS = GetPlayerState<AZeroDawnPlayerState>();
	if (!ZDPS)
	{
		return;
	}

	ETeamType MyTeam = ZDPS->PlayerTeam;

	// Iterate all player states to find alive teammates
	for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
	{
		AZeroDawnPlayerState* OtherPS = Cast<AZeroDawnPlayerState>(PS);
		if (!OtherPS || OtherPS == ZDPS)
		{
			continue;
		}

		// Must be on the same team
		if (OtherPS->PlayerTeam != MyTeam)
		{
			continue;
		}

		// Get their pawn and check if alive
		AController* OtherPC = Cast<AController>(OtherPS->GetOwner());
		if (!OtherPC)
		{
			continue;
		}

		AZeroDawnCharacter* OtherChar = Cast<AZeroDawnCharacter>(OtherPC->GetPawn());
		if (OtherChar && OtherChar->IsAlive())
		{
			SpectateTargets.Add(OtherChar);
		}
	}
}

void AZeroDawnPlayerController::UpdateSpectatorOverlay()
{
	if (!bIsSpectating || !IsLocalController() || !SpectatorOverlay)
	{
		return;
	}

	// Get the currently observed character
	AZeroDawnCharacter* ObservedChar = nullptr;
	if (SpectateTargets.IsValidIndex(CurrentSpectateTargetIndex))
	{
		ObservedChar = SpectateTargets[CurrentSpectateTargetIndex].Get();
	}

	if (!ObservedChar || !IsValid(ObservedChar))
	{
		// The observed pawn may have died; try to refresh and find a valid one
		RefreshSpectateTargets();
		if (SpectateTargets.Num() > 0)
		{
			UpdateSpectateViewTarget(0);
			ObservedChar = SpectateTargets[0].Get();
		}
		else
		{
			SpectatorOverlay->HideOverlay();
			return;
		}
	}

	// Get the observed player's name from their PlayerState
	FString PlayerName = TEXT("Unknown");
	AZeroDawnPlayerState* ObservedPS = ObservedChar->GetPlayerState<AZeroDawnPlayerState>();
	if (ObservedPS)
	{
		PlayerName = ObservedPS->GetPlayerName();
	}

	// Update the overlay with observed player's health info
	SpectatorOverlay->UpdateObservedInfo(PlayerName, ObservedChar->CurrentHealth, ObservedChar->MaxHealth);
	SpectatorOverlay->ShowOverlay();
}

void AZeroDawnPlayerController::UpdateSpectateViewTarget(int32 Index)
{
	if (!SpectateTargets.IsValidIndex(Index))
	{
		return;
	}

	AZeroDawnCharacter* TargetChar = SpectateTargets[Index].Get();
	if (!TargetChar || !IsValid(TargetChar))
	{
		return;
	}

	CurrentSpectateTargetIndex = Index;

	// Set the view target to the observed character
	// Use the character's third-person camera or default view
	SetViewTargetWithBlend(TargetChar, 0.3f, VTBlend_Cubic, 0.0f, false);

	// Update overlay immediately
	if (IsLocalController() && SpectatorOverlay)
	{
		AZeroDawnPlayerState* ObservedPS = TargetChar->GetPlayerState<AZeroDawnPlayerState>();
		FString PlayerName = ObservedPS ? ObservedPS->GetPlayerName() : TEXT("Unknown");
		SpectatorOverlay->UpdateObservedInfo(PlayerName, TargetChar->CurrentHealth, TargetChar->MaxHealth);
		SpectatorOverlay->ShowOverlay();
	}

	UE_LOG(LogTemp, Verbose, TEXT("Spectator: Now observing teammate #%d"), Index);
}
