#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnWeaponInventory.generated.h"

class AZeroDawnWeapon;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnWeaponInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnWeaponInventory();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwitchWeapon();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectWeapon(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PickupWeapon(AZeroDawnWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropCurrentWeapon();

	/** Checks whether a weapon of the given type is allowed in the specified slot.
	 *  Slot 0 (primary) accepts any weapon type.
	 *  Slot 1 (secondary) only accepts Pistol/SMG unless the Overkill perk is active. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsWeaponAllowedInSlot(EWeaponType WeaponType, int32 SlotIndex) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxWeaponSlots = 2;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<AZeroDawnWeapon*> InventorySlots;

protected:
	class UZeroDawnWeaponComponent* GetWeaponComponent() const;

	/** Returns the owning character, or nullptr if the owner is not an AZeroDawnCharacter. */
	class AZeroDawnCharacter* GetOwnerCharacter() const;
};
