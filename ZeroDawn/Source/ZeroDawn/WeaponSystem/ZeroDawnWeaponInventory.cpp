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
