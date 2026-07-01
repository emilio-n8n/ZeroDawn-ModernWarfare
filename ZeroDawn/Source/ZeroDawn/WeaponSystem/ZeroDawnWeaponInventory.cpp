#include "ZeroDawnWeaponInventory.h"
#include "ZeroDawnWeaponComponent.h"
#include "../Weapon/ZeroDawnWeapon.h"
#include "../Character/ZeroDawnCharacter.h"

UZeroDawnWeaponInventory::UZeroDawnWeaponInventory()
{
	SetIsReplicatedByDefault(true);
}

void UZeroDawnWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.Reserve(MaxWeaponSlots);
}

void UZeroDawnWeaponInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UZeroDawnWeaponInventory, InventorySlots, COND_None);
}

UZeroDawnWeaponComponent* UZeroDawnWeaponInventory::GetWeaponComponent() const
{
	return GetOwner()->FindComponentByClass<UZeroDawnWeaponComponent>();
}

AZeroDawnCharacter* UZeroDawnWeaponInventory::GetOwnerCharacter() const
{
	return Cast<AZeroDawnCharacter>(GetOwner());
}

bool UZeroDawnWeaponInventory::IsWeaponAllowedInSlot(EWeaponType WeaponType, int32 SlotIndex) const
{
	// Slot 0 (primary): any weapon type is allowed
	if (SlotIndex == 0)
	{
		return true;
	}

	// Slot 1 (secondary): restricted without Overkill
	if (SlotIndex == 1)
	{
		AZeroDawnCharacter* OwnerChar = GetOwnerCharacter();
		if (OwnerChar && OwnerChar->bOverkill)
		{
			// Overkill perk active: allow any weapon type in secondary slot
			return true;
		}

		// Without Overkill, only Pistol and SMG are allowed in secondary slot
		return (WeaponType == EWeaponType::Pistol || WeaponType == EWeaponType::SMG);
	}

	// Any other slot index is unrestricted
	return true;
}

void UZeroDawnWeaponInventory::SwitchWeapon()
{
	int32 CurrentIndex = 0;
	UZeroDawnWeaponComponent* WC = GetWeaponComponent();
	if (WC)
	{
		CurrentIndex = WC->CurrentWeaponIndex;
		int32 NextIndex = (CurrentIndex + 1) % FMath::Max(1, WC->Weapons.Num());
		WC->EquipWeapon(NextIndex);
	}
}

void UZeroDawnWeaponInventory::SelectWeapon(int32 SlotIndex)
{
	UZeroDawnWeaponComponent* WC = GetWeaponComponent();
	if (WC && InventorySlots.IsValidIndex(SlotIndex))
	{
		int32 WeaponIdx = WC->Weapons.IndexOfByKey(InventorySlots[SlotIndex]);
		if (WeaponIdx != INDEX_NONE)
		{
			WC->EquipWeapon(WeaponIdx);
		}
	}
}

void UZeroDawnWeaponInventory::PickupWeapon(AZeroDawnWeapon* Weapon)
{
	if (!Weapon || !GetOwner() || !GetOwner()->HasAuthority()) return;

	// Determine which slot this weapon would occupy
	int32 NewSlotIndex = InventorySlots.Num();
	if (NewSlotIndex >= MaxWeaponSlots)
	{
		// Inventory is full — we will drop the current weapon first, then add
		// to the slot that was just freed (slot 1 after dropping from slot 0 or 1).
		// After DropCurrentWeapon, InventorySlots.Num() will be 1 (if 2 weapons)
		// so the new weapon goes to slot index 1.
		NewSlotIndex = MaxWeaponSlots - 1;
	}

	// Check slot restrictions before picking up
	if (!IsWeaponAllowedInSlot(Weapon->WeaponStats.WeaponType, NewSlotIndex))
	{
		return;
	}

	if (InventorySlots.Num() < MaxWeaponSlots)
	{
		InventorySlots.Add(Weapon);
	}
	else
	{
		DropCurrentWeapon();
		InventorySlots.Add(Weapon);
	}
}

void UZeroDawnWeaponInventory::DropCurrentWeapon()
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	UZeroDawnWeaponComponent* WC = GetWeaponComponent();
	if (WC)
	{
		AZeroDawnWeapon* CurrentWeapon = WC->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentWeapon->SetActorEnableCollision(true);
			CurrentWeapon->SetLifeSpan(30.0f);
			InventorySlots.Remove(CurrentWeapon);
			WC->Weapons.Remove(CurrentWeapon);
		}
	}
}
