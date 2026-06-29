#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnWeaponComponent.generated.h"

class AZeroDawnWeapon;
class AZeroDawnCharacter;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnWeaponComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TArray<TSubclassOf<AZeroDawnWeapon>> DefaultWeaponClasses;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<AZeroDawnWeapon*> Weapons;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	int32 CurrentWeaponIndex = 0;

	AZeroDawnWeapon* GetCurrentWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void EquipWeapon(int32 WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void SpawnDefaultWeapons();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ReloadCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void MeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void HideWeapons();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShowWeapons();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	float MeleeDamage = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	float MeleeRange = 150.0f;

protected:
	AZeroDawnCharacter* GetOwnerCharacter() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquipWeapon(int32 WeaponIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnDefaultWeapons();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMeleeAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon(int32 WeaponIndex);

	void UnequipCurrentWeapon();
	void EquipWeaponInternal(AZeroDawnWeapon* Weapon);
};
