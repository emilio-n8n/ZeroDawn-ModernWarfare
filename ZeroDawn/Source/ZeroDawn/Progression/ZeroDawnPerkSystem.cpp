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
	return BaseValue;
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

void UZeroDawnPerkSystem::ApplyEOD() {}

void UZeroDawnPerkSystem::ApplyDoubleTime()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->SprintSpeed *= 1.1f;
	}
}

void UZeroDawnPerkSystem::ApplyColdBlooded() {}
void UZeroDawnPerkSystem::ApplyHardline()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerChar->KillstreakManager->CheckKillstreaks(OwnerChar->CurrentKillstreak);
	}
}

void UZeroDawnPerkSystem::ApplyQuickFix() {}
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

void UZeroDawnPerkSystem::ApplyNimble() {}
void UZeroDawnPerkSystem::ApplyGhost() {}
void UZeroDawnPerkSystem::ApplyOverkill() {}
