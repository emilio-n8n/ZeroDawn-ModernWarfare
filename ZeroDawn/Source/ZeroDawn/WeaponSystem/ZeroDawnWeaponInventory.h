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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxWeaponSlots = 2;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<AZeroDawnWeapon*> InventorySlots;

protected:
	class UZeroDawnWeaponComponent* GetWeaponComponent() const;
};
