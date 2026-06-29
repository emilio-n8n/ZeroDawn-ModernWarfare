#include "ZeroDawnCreateAClass.h"

UZeroDawnCreateAClass::UZeroDawnCreateAClass()
{
	CurrentLoadout.ClassName = "Default";
	CurrentLoadout.PrimaryWeapon = EWeaponType::AssaultRifle;
	CurrentLoadout.SecondaryWeapon = EWeaponType::Pistol;
	CurrentLoadout.Perk1 = EPerkType::FlakJacket;
	CurrentLoadout.Perk2 = EPerkType::ColdBlooded;
	CurrentLoadout.Perk3 = EPerkType::Amped;

	SavedLoadouts.SetNum(5);
	for (int32 i = 0; i < 5; i++)
	{
		SavedLoadouts[i] = CurrentLoadout;
		SavedLoadouts[i].ClassName = FString::Printf(TEXT("Class %d"), i + 1);
	}
}

void UZeroDawnCreateAClass::SetLoadout(const FLoadoutClass& NewLoadout)
{
	CurrentLoadout = NewLoadout;
}

void UZeroDawnCreateAClass::SetPrimaryWeapon(EWeaponType Weapon)
{
	CurrentLoadout.PrimaryWeapon = Weapon;
}

void UZeroDawnCreateAClass::SetSecondaryWeapon(EWeaponType Weapon)
{
	CurrentLoadout.SecondaryWeapon = Weapon;
}

void UZeroDawnCreateAClass::SetPerk(EPerkSlot Slot, EPerkType Perk)
{
	switch (Slot)
	{
	case EPerkSlot::Perk1: CurrentLoadout.Perk1 = Perk; break;
	case EPerkSlot::Perk2: CurrentLoadout.Perk2 = Perk; break;
	case EPerkSlot::Perk3: CurrentLoadout.Perk3 = Perk; break;
	}
}

void UZeroDawnCreateAClass::AddAttachment(EWeaponType ForWeapon, const FWeaponAttachment& Attachment)
{
	CurrentLoadout.Attachments.Add(Attachment);
}

void UZeroDawnCreateAClass::SaveLoadout(int32 Slot)
{
	if (SavedLoadouts.IsValidIndex(Slot))
	{
		SavedLoadouts[Slot] = CurrentLoadout;
	}
}

void UZeroDawnCreateAClass::LoadLoadout(int32 Slot)
{
	if (SavedLoadouts.IsValidIndex(Slot))
	{
		CurrentLoadout = SavedLoadouts[Slot];
	}
}
