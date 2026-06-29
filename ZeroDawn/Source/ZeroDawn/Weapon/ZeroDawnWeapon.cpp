#include "ZeroDawnWeapon.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../WeaponSystem/ZeroDawnWeaponComponent.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "Engine/DamageEvents.h"

AZeroDawnWeapon::AZeroDawnWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->SetOwnerNoSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);

	bReplicates = true;
	SetReplicateMovement(false);
	SetNetUpdateFrequency(50.0f);

	CurrentAmmo = 30;
	ReserveAmmo = 210;
	CurrentFireMode = EFireMode::Automatic;
	CurrentSpread = 0.0f;
	CurrentRecoilStep = 0;
}

void AZeroDawnWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = WeaponStats.MagazineSize;
	ReserveAmmo = MaxReserveAmmo;
	CurrentRecoilStep = 0;
	CurrentSpread = 0.0f;
}

void AZeroDawnWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSpread > 0.0f)
	{
		CurrentSpread = FMath::Max(0.0f, CurrentSpread - (DeltaTime * 5.0f));
	}
}

void AZeroDawnWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AZeroDawnWeapon, Attachments, COND_OwnerOnly);
	DOREPLIFETIME(AZeroDawnWeapon, CurrentAmmo);
	DOREPLIFETIME(AZeroDawnWeapon, WeaponStats);
}

AZeroDawnCharacter* AZeroDawnWeapon::GetOwningCharacter() const
{
	return Cast<AZeroDawnCharacter>(GetOwner());
}

void AZeroDawnWeapon::StartFire()
{
	if (bIsReloading || CurrentAmmo <= 0) return;
	bIsFiring = true;

	FireWeapon();

	if (WeaponStats.bIsAutomatic)
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AZeroDawnWeapon::FireWeapon, WeaponStats.FireRate, true);
	}
}

void AZeroDawnWeapon::StopFire()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AZeroDawnWeapon::ReloadWeapon()
{
	if (bIsReloading || CurrentAmmo >= WeaponStats.MagazineSize || ReserveAmmo <= 0) return;

	bIsReloading = true;
	MulticastPlayReloadEffects();

	float ActualReloadTime = WeaponStats.ReloadTime;

	for (const FWeaponAttachment& Attach : Attachments)
	{
		ActualReloadTime -= Attach.ReloadSpeedBonus;
	}
	ActualReloadTime = FMath::Max(0.5f, ActualReloadTime);

	FTimerHandle ReloadHandle;
	GetWorldTimerManager().SetTimer(ReloadHandle, [this]()
	{
		int32 AmmoNeeded = WeaponStats.MagazineSize - CurrentAmmo;
		int32 AmmoToReload = FMath::Min(AmmoNeeded, ReserveAmmo);
		CurrentAmmo += AmmoToReload;
		ReserveAmmo -= AmmoToReload;
		bIsReloading = false;
	}, ActualReloadTime, false);
}

void AZeroDawnWeapon::FireWeapon()
{
	if (bIsReloading || CurrentAmmo <= 0)
	{
		if (CurrentAmmo <= 0)
		{
			if (EmptySound) UGameplayStatics::PlaySoundAtLocation(this, EmptySound, GetActorLocation());
			StopFire();
		}
		return;
	}

	CurrentAmmo--;

	FVector ShotDirection;
	float ShotDamage;
	CalculateShot(ShotDirection, ShotDamage);

	FVector StartLocation = MuzzleLocation ? MuzzleLocation->GetComponentLocation() : GetActorLocation();

	if (HasAuthority())
	{
		ServerFireShot(StartLocation, ShotDirection, ShotDamage);
	}
	else
	{
		ServerFireShot(StartLocation, ShotDirection, ShotDamage);
	}

	MulticastPlayFireEffects();

	if (!WeaponStats.bIsAutomatic)
	{
		StopFire();
	}

	CurrentRecoilStep++;
}

void AZeroDawnWeapon::CalculateShot(FVector& OutDirection, float& OutDamage)
{
	AZeroDawnCharacter* OwnerChar = GetOwningCharacter();
	if (!OwnerChar)
	{
		OutDirection = GetActorForwardVector();
		OutDamage = WeaponStats.Damage;
		return;
	}

	FVector CamLocation;
	FRotator CamRotation;

	if (OwnerChar->FirstPersonCameraComponent)
	{
		CamLocation = OwnerChar->FirstPersonCameraComponent->GetComponentLocation();
		CamRotation = OwnerChar->FirstPersonCameraComponent->GetComponentRotation();
	}
	else
	{
		CamLocation = OwnerChar->GetActorLocation() + FVector(0, 0, 64);
		CamRotation = OwnerChar->GetActorRotation();
	}

	float TotalSpread = CurrentSpread;

	if (!OwnerChar->bIsADS)
	{
		TotalSpread += WeaponStats.Spread + (CurrentSpread * HipFireSpreadMultiplier);
	}
	else
	{
		TotalSpread += WeaponStats.SpreadADS + (CurrentSpread * 0.5f);
	}

	TotalSpread = FMath::Clamp(TotalSpread, 0.0f, 10.0f);

	float RandYaw = FMath::FRandRange(-TotalSpread, TotalSpread);
	float RandPitch = FMath::FRandRange(-TotalSpread, TotalSpread);
	FRotator SpreadRotator = CamRotation + FRotator(RandPitch, RandYaw, 0);

	OutDirection = SpreadRotator.Vector();

	OutDamage = WeaponStats.Damage;

	float Distance = FVector::Dist(CamLocation, GetActorLocation());
	if (Distance > WeaponStats.DamageFalloffStart)
	{
		float FalloffAlpha = FMath::Clamp((Distance - WeaponStats.DamageFalloffStart) / (WeaponStats.DamageFalloffEnd - WeaponStats.DamageFalloffStart), 0.0f, 1.0f);
		OutDamage *= FMath::Lerp(1.0f, WeaponStats.DamageFalloffMultiplier, FalloffAlpha);
	}

	CurrentSpread += SpreadIncreasePerShot;
}

bool AZeroDawnWeapon::ServerFireShot_Validate(FVector_NetQuantize StartLocation, FVector_NetQuantize Direction, float Damage) { return true; }
void AZeroDawnWeapon::ServerFireShot_Implementation(FVector_NetQuantize StartLocation, FVector_NetQuantize Direction, float Damage)
{
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = true;

	FVector EndLocation = StartLocation + (Direction * WeaponStats.Range);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	if (bHit && Hit.GetActor())
	{
		FPointDamageEvent DamageEvent(Damage, Hit, -Direction, nullptr);

		if (Hit.BoneName.ToString().Contains("head", ESearchCase::IgnoreCase) ||
			Hit.BoneName.ToString().Contains("neck", ESearchCase::IgnoreCase))
		{
			Damage *= WeaponStats.HeadshotMultiplier;
		}

		Hit.GetActor()->TakeDamage(Damage, DamageEvent, GetOwner()->GetInstigatorController(), this);

		AZeroDawnCharacter* OwnerChar = GetOwningCharacter();
		if (OwnerChar)
		{
			OwnerChar->MulticastPlayHitEffect(Hit.Location, Hit.ImpactNormal.Rotation());
		}
	}

	if (TrailEffect && bHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailEffect, Hit.Location);
	}
}

void AZeroDawnWeapon::MulticastPlayFireEffects_Implementation()
{
	if (MuzzleFlashEffect && MuzzleLocation)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, WeaponMesh, MuzzleLocation->GetAttachSocketName());
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation ? MuzzleLocation->GetComponentLocation() : GetActorLocation());
	}

	if (FireAnimation && WeaponMesh)
	{
		WeaponMesh->GetAnimInstance()->Montage_Play(FireAnimation);
	}
}

void AZeroDawnWeapon::MulticastPlayReloadEffects_Implementation()
{
	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}

	if (ReloadAnimation && WeaponMesh)
	{
		WeaponMesh->GetAnimInstance()->Montage_Play(ReloadAnimation);
	}
}

void AZeroDawnWeapon::ApplyRecoil()
{
	if (!GetOwningCharacter() || !GetOwningCharacter()->IsLocallyControlled()) return;

	APlayerController* PC = Cast<APlayerController>(GetOwningCharacter()->GetController());
	if (!PC) return;

	if (RecoilPattern.Num() > 0)
	{
		int32 PatternIdx = FMath::Min(CurrentRecoilStep, RecoilPattern.Num() - 1);
		FVector2D RecoilAmount = RecoilPattern[PatternIdx];

		float RecoilMultiplier = WeaponStats.RecoilVertical;
		PC->AddPitchInput(-RecoilAmount.X * RecoilMultiplier * 0.01f);
		PC->AddYawInput(RecoilAmount.Y * WeaponStats.RecoilHorizontal * 0.01f);
	}
	else
	{
		PC->AddPitchInput(-FMath::FRandRange(0.5f, 1.5f) * WeaponStats.RecoilVertical * 0.01f);
		PC->AddYawInput(FMath::FRandRange(-0.5f, 0.5f) * WeaponStats.RecoilHorizontal * 0.01f);
	}
}

void AZeroDawnWeapon::SetRecoilPattern(int32 PatternIndex)
{
	CurrentRecoilStep = PatternIndex;
}
