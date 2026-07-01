#include "ZeroDawnCreateAClass.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "ZeroDawnSaveGame.h"

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
	if (!SavedLoadouts.IsValidIndex(Slot)) return;

	// Update in-memory array
	SavedLoadouts[Slot] = CurrentLoadout;

	// Persist to disk via SaveGame system
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	if (OwnerActor->HasAuthority())
	{
		// Server path: trigger Client RPC to save on the client (where filesystem is available)
		AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OwnerActor);
		if (Char)
		{
			AZeroDawnPlayerState* PS = Char->GetPlayerState<AZeroDawnPlayerState>();
			if (PS)
			{
				PS->ClientSaveGameData();
			}
		}
	}
	else
	{
		// Client path: write directly to SaveGame
		AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OwnerActor);
		if (Char)
		{
			AZeroDawnPlayerState* PS = Char->GetPlayerState<AZeroDawnPlayerState>();
			if (PS)
			{
				const FString PlayerId = PS->GetSavePlayerId();

				// Load existing save data to preserve other fields
				UZeroDawnSaveGame* SaveData = UZeroDawnSaveGame::LoadProgress(this, PlayerId);
				if (!SaveData)
				{
					SaveData = NewObject<UZeroDawnSaveGame>();
				}

				// Write loadout data
				SaveData->SavedLoadouts = SavedLoadouts;

				// Persist to disk
				UZeroDawnSaveGame::SaveProgress(this, PlayerId, SaveData);
			}
		}
	}
}

void UZeroDawnCreateAClass::LoadLoadout(int32 Slot)
{
	if (!SavedLoadouts.IsValidIndex(Slot)) return;

	// First try to load the most recent data from disk
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OwnerActor);
		if (Char)
		{
			AZeroDawnPlayerState* PS = Char->GetPlayerState<AZeroDawnPlayerState>();
			if (PS)
			{
				const FString PlayerId = PS->GetSavePlayerId();
				UZeroDawnSaveGame* SaveData = UZeroDawnSaveGame::LoadProgress(this, PlayerId);
				if (SaveData && SaveData->SavedLoadouts.IsValidIndex(Slot))
				{
					// Restore from disk-saved data
					SavedLoadouts = SaveData->SavedLoadouts;
					CurrentLoadout = SavedLoadouts[Slot];
					return;
				}
			}
		}
	}

	// Fallback: load from in-memory array (which was populated from disk during BeginPlay)
	CurrentLoadout = SavedLoadouts[Slot];
}
