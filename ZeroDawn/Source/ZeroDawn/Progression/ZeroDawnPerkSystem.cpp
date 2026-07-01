#include "ZeroDawnPerkSystem.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Progression/ZeroDawnKillstreakManager.h"
#include "../WeaponSystem/ZeroDawnWeaponComponent.h"
#include "../Weapon/ZeroDawnWeapon.h"

UZeroDawnPerkSystem::UZeroDawnPerkSystem()
{
	SetIsReplicatedByDefault(true);
}

void UZeroDawnPerkSystem::ApplyPerks(const FLoadoutClass& Loadout)
{
	ActivePerks.Empty();
	ActivePerks.Add(Loadout.Perk1);
	ActivePerks.Add(Loadout.Perk2);
	ActivePerks.Add(Loadout.Perk3);

	for (EPerkType Perk : ActivePerks)
	{
		ApplyPerkEffect(Perk);
	}
}

bool UZeroDawnPerkSystem::HasPerk(EPerkType Perk) const
{
	return ActivePerks.Contains(Perk);
}

float UZeroDawnPerkSystem::GetPerkModifiedValue(float BaseValue, EPerkType RelevantPerk) const
{
	if (!HasPerk(RelevantPerk)) return BaseValue;

	switch (RelevantPerk)
	{
	case EPerkType::FlakJacket:
		// Flak Jacket: bonus armor capacity (MaxArmor = 75), no direct value modification
		return BaseValue;

	case EPerkType::EOD:
		// EOD: explosive damage taken reduced by 50%
		return BaseValue * 0.5f;

	case EPerkType::DoubleTime:
		// Double Time: sprint speed increased by 10%
		return BaseValue * 1.1f;

	case EPerkType::ColdBlooded:
		// ColdBlooded: invisible to AI/thermal, no numerical modifier
		return BaseValue;

	case EPerkType::Hardline:
		// Hardline: killstreak requires 1 fewer kill
		return BaseValue - 1.0f;

	case EPerkType::QuickFix:
		// QuickFix: health regen delay reduced by 60% (from 5s to 2s)
		return BaseValue * 0.4f;

	case EPerkType::Amped:
		// Amped: reload speed increased by 25%
		return BaseValue * 0.75f;

	case EPerkType::Nimble:
		// Nimble: ADS movement speed increased
		return BaseValue * 1.35f;

	case EPerkType::Ghost:
		// Ghost: invisible to UAV, no numerical modifier
		return BaseValue;

	case EPerkType::Overkill:
		// Overkill: two primary weapons, no numerical modifier
		return BaseValue;

	default:
		return BaseValue;
	}
}

void UZeroDawnPerkSystem::ApplyPerkEffect(EPerkType Perk)
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (!OwnerChar) return;

	switch (Perk)
	{
	case EPerkType::FlakJacket: ApplyFlakJacket(); break;
	case EPerkType::EOD: ApplyEOD(); break;
	case EPerkType::DoubleTime: ApplyDoubleTime(); break;
	case EPerkType::ColdBlooded: ApplyColdBlooded(); break;
	case EPerkType::Hardline: ApplyHardline(); break;
	case EPerkType::QuickFix: ApplyQuickFix(); break;
	case EPerkType::Amped: ApplyAmped(); break;
	case EPerkType::Nimble: ApplyNimble(); break;
	case EPerkType::Ghost: ApplyGhost(); break;
	case EPerkType::Overkill: ApplyOverkill(); break;
	}
}

void UZeroDawnPerkSystem::ApplyFlakJacket()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->MaxArmor = 75.0f;
	}
}

void UZeroDawnPerkSystem::ApplyEOD()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->ExplosiveDamageMultiplier = 0.5f;
	}
}

void UZeroDawnPerkSystem::ApplyDoubleTime()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->SprintSpeed *= 1.1f;
	}
}

void UZeroDawnPerkSystem::ApplyColdBlooded()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->bColdBlooded = true;
	}
}
void UZeroDawnPerkSystem::ApplyHardline()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->KillstreakManager->CheckKillstreaks(OwnerChar->CurrentKillstreak);
	}
}

void UZeroDawnPerkSystem::ApplyQuickFix()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		// Reduce health regen delay from 5s to 2s
		OwnerChar->HealthRegenDelay = 2.0f;
	}
}
void UZeroDawnPerkSystem::ApplyAmped()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && OwnerChar->WeaponComponent)
	{
		for (auto* Weapon : OwnerChar->WeaponComponent->Weapons)
		{
			if (Weapon)
			{
				Weapon->WeaponStats.ReloadTime *= 0.75f;
			}
		}
	}
}

void UZeroDawnPerkSystem::ApplyNimble()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		// Increase ADS movement speed from 0.6x to 0.85x of walk speed
		OwnerChar->ADSSpeedMultiplier = 0.85f;
	}
}
void UZeroDawnPerkSystem::ApplyGhost()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->bGhost = true;
	}
}
void UZeroDawnPerkSystem::ApplyOverkill()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->bOverkill = true;
	}
}
