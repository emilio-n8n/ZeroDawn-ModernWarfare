#include "ZeroDawnKillstreakManager.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../UI/ZeroDawnKillstreakHUD.h"
#include "../WeaponSystem/ZeroDawnCameraShake.h"

UZeroDawnKillstreakManager::UZeroDawnKillstreakManager()
{
	SetIsReplicatedByDefault(true);

	KillstreakRequirements =
	{
		{EKillstreakType::UAV, 3},
		{EKillstreakType::CarePackage, 5},
		{EKillstreakType::AttackHelicopter, 7},
		{EKillstreakType::Gunship, 10},
		{EKillstreakType::Nuke, 15}
	};
}

void UZeroDawnKillstreakManager::CheckKillstreaks(int32 CurrentKillstreak)
{
	for (const auto& Req : KillstreakRequirements)
	{
		if (CurrentKillstreak >= Req.Value)
		{
			OnKillstreakAwarded(Req.Key);
		}
	}

	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		UZeroDawnKillstreakHUD* KHUD = OwnerChar->FindComponentByClass<UZeroDawnKillstreakHUD>();
		if (KHUD)
		{
			KHUD->ClientShowStreak(CurrentKillstreak);
		}

		APlayerController* PC = Cast<APlayerController>(OwnerChar->GetController());
		if (PC)
		{
			UZeroDawnCameraShake::PlayKillstreakShake(PC);
		}
	}
}

void UZeroDawnKillstreakManager::RequestKillstreak(EKillstreakType Type)
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->IsAlive()) return;

	switch (Type)
	{
	case EKillstreakType::UAV:
	{
		// Reveal enemies on minimap (skip players with Ghost perk)
		if (!OwnerChar->HasAuthority()) return;

		for (TActorIterator<AZeroDawnCharacter> It(GetWorld()); It; ++It)
		{
			AZeroDawnCharacter* TargetChar = *It;
			if (!TargetChar || TargetChar == OwnerChar) continue;
			if (TargetChar->TeamType == OwnerChar->TeamType) continue;
			if (TargetChar->bGhost) continue;

			// Mark on minimap: set a replicated flag or call minimap HUD update
			APlayerController* TargetPC = Cast<APlayerController>(TargetChar->GetController());
			if (TargetPC && TargetPC->IsLocalController())
			{
				// The UAV detection is handled via HUD/minimap widget updates
				// that read from GameState or a manager at runtime.
				// Ghost perk check ensures bGhost players are excluded.
			}
		}
		break;
	}
	case EKillstreakType::CarePackage:
		// Spawn a care package at a location
		break;
	case EKillstreakType::AttackHelicopter:
		// Spawn an AI helicopter that attacks enemies
		break;
	case EKillstreakType::Gunship:
		// Player controls an AC-130 gunship for a duration
		break;
	case EKillstreakType::Nuke:
		// End the match - player's team wins
		break;
	}
}
