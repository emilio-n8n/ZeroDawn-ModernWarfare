#include "ZeroDawnWeaponComponent.h"
#include "../Weapon/ZeroDawnWeapon.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../WeaponSystem/ZeroDawnWeaponInventory.h"
#include "Engine/DamageEvents.h"

UZeroDawnWeaponComponent::UZeroDawnWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	SetIsReplicatedByDefault(true);
}

void UZeroDawnWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UZeroDawnWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UZeroDawnWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UZeroDawnWeaponComponent, Weapons, COND_None);
	DOREPLIFETIME_CONDITION(UZeroDawnWeaponComponent, CurrentWeaponIndex, COND_OwnerOnly);
}

AZeroDawnCharacter* UZeroDawnWeaponComponent::GetOwnerCharacter() const
{
	return Cast<AZeroDawnCharacter>(GetOwner());
}

void UZeroDawnWeaponComponent::SpawnDefaultWeapons()
{
	ServerSpawnDefaultWeapons();
}

bool UZeroDawnWeaponComponent::ServerSpawnDefaultWeapons_Validate() { return true; }
void UZeroDawnWeaponComponent::ServerSpawnDefaultWeapons_Implementation()
{
	for (auto& WeaponClass : DefaultWeaponClasses)
	{
		if (!WeaponClass) continue;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AZeroDawnWeapon* NewWeapon = GetWorld()->SpawnActor<AZeroDawnWeapon>(WeaponClass, FTransform::Identity, SpawnParams);
		if (NewWeapon)
		{
			NewWeapon->SetOwner(GetOwner());
			Weapons.Add(NewWeapon);
		}
	}

	if (Weapons.Num() > 0)
	{
		EquipWeapon(0);
	}
}

AZeroDawnWeapon* UZeroDawnWeaponComponent::GetCurrentWeapon() const
{
	if (Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		return Weapons[CurrentWeaponIndex];
	}
	return nullptr;
}

void UZeroDawnWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (!Weapons.IsValidIndex(WeaponIndex)) return;
	ServerEquipWeapon(WeaponIndex);
}

bool UZeroDawnWeaponComponent::ServerEquipWeapon_Validate(int32 WeaponIndex) { return true; }
void UZeroDawnWeaponComponent::ServerEquipWeapon_Implementation(int32 WeaponIndex)
{
	if (!Weapons.IsValidIndex(WeaponIndex)) return;

	UnequipCurrentWeapon();
	CurrentWeaponIndex = WeaponIndex;
	EquipWeaponInternal(Weapons[WeaponIndex]);
	MulticastEquipWeapon(WeaponIndex);
}

void UZeroDawnWeaponComponent::MulticastEquipWeapon_Implementation(int32 WeaponIndex)
{
	if (!Weapons.IsValidIndex(WeaponIndex)) return;

	UnequipCurrentWeapon();
	CurrentWeaponIndex = WeaponIndex;
	EquipWeaponInternal(Weapons[WeaponIndex]);
}

void UZeroDawnWeaponComponent::UnequipCurrentWeapon()
{
	AZeroDawnWeapon* Current = GetCurrentWeapon();
	if (Current)
	{
		Current->SetActorHiddenInGame(true);
		Current->SetActorEnableCollision(false);
		if (Current->GetAttachParentActor())
		{
			Current->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void UZeroDawnWeaponComponent::EquipWeaponInternal(AZeroDawnWeapon* Weapon)
{
	if (!Weapon) return;

	AZeroDawnCharacter* OwnerChar = GetOwnerCharacter();
	if (!OwnerChar) return;

	Weapon->SetActorHiddenInGame(false);
	Weapon->SetActorEnableCollision(false);

	FName AttachSocket = TEXT("WeaponSocket");
	Weapon->AttachToComponent(OwnerChar->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
	Weapon->SetOwner(OwnerChar);
}

void UZeroDawnWeaponComponent::StartFire()
{
	AZeroDawnWeapon* Weapon = GetCurrentWeapon();
	if (Weapon) Weapon->StartFire();
}

void UZeroDawnWeaponComponent::StopFire()
{
	AZeroDawnWeapon* Weapon = GetCurrentWeapon();
	if (Weapon) Weapon->StopFire();
}

void UZeroDawnWeaponComponent::ReloadCurrentWeapon()
{
	AZeroDawnWeapon* Weapon = GetCurrentWeapon();
	if (Weapon) Weapon->ReloadWeapon();
}

void UZeroDawnWeaponComponent::MeleeAttack()
{
	ServerMeleeAttack();
}

bool UZeroDawnWeaponComponent::ServerMeleeAttack_Validate() { return true; }
void UZeroDawnWeaponComponent::ServerMeleeAttack_Implementation()
{
	AZeroDawnCharacter* OwnerChar = GetOwnerCharacter();
	if (!OwnerChar) return;

	FVector StartLocation = OwnerChar->FirstPersonCameraComponent->GetComponentLocation();
	FVector Forward = OwnerChar->FirstPersonCameraComponent->GetForwardVector();
	FVector EndLocation = StartLocation + (Forward * MeleeRange);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerChar);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	if (bHit && Hit.GetActor())
	{
		FPointDamageEvent DamageEvent(MeleeDamage, Hit, -Forward, nullptr);
		Hit.GetActor()->TakeDamage(MeleeDamage, DamageEvent, OwnerChar->GetController(), OwnerChar);
	}
}

void UZeroDawnWeaponComponent::HideWeapons()
{
	for (auto* Weapon : Weapons)
	{
		if (Weapon) Weapon->SetActorHiddenInGame(true);
	}
}

void UZeroDawnWeaponComponent::ShowWeapons()
{
	AZeroDawnWeapon* Current = GetCurrentWeapon();
	if (Current) Current->SetActorHiddenInGame(false);
}
