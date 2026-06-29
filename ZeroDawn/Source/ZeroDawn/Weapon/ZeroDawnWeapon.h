#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnWeapon.generated.h"

class AZeroDawnCharacter;

UCLASS()
class AZeroDawnWeapon : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Weapon")
	FWeaponStats WeaponStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 ReserveAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EFireMode CurrentFireMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsReloading = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 MaxReserveAmmo = 210;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USoundCue* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USoundCue* EmptySound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<FWeaponAttachment> Attachments;

	AZeroDawnCharacter* GetOwningCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon();

	void SetRecoilPattern(int32 PatternIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentRecoilStep = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FVector2D> RecoilPattern;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RecoilResetSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RecoilRecoveryDelay = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HipFireSpreadMultiplier = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float SpreadIncreasePerShot = 0.05f;

	float CurrentSpread;
	FTimerHandle FireTimerHandle;

	void ApplyRecoil();
	void CalculateShot(FVector& OutDirection, float& OutDamage);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* ReloadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;

	short WeaponID;

	bool bUsePhysicsProjectile = false;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireShot(FVector_NetQuantize StartLocation, FVector_NetQuantize Direction, float Damage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayFireEffects();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayReloadEffects();
};
