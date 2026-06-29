#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "WeaponDataTable.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::AssaultRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 28.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float HeadshotMultiplier = 1.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineSize = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime = 2.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ADSInTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float MOVEMENT_SPEED = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float HIP_SPREAD = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ADS_SPREAD = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RECOIL_VERTICAL = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RECOIL_HORIZONTAL = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 RANGE_METER = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString DamageDropoff = "28-22";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BulletVelocity = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float SprintToFireTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AimAssistRange = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString RecoilPattern = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString Description;
};

static TMap<EWeaponType, FWeaponDataRow> GetDefaultWeaponData()
{
	TMap<EWeaponType, FWeaponDataRow> Data;

	// M4A1 - Assault Rifle (balanced, accurate)
	{
		FWeaponDataRow W;
		W.WeaponName = "M4A1"; W.WeaponType = EWeaponType::AssaultRifle;
		W.Damage = 28; W.HeadshotMultiplier = 1.5f; W.FireRate = 700;
		W.MagazineSize = 30; W.ReloadTime = 2.2f; W.ADSInTime = 0.25f;
		W.HIP_SPREAD = 4.0f; W.ADS_SPREAD = 1.0f;
		W.RECOIL_VERTICAL = 30; W.RECOIL_HORIZONTAL = 10;
		W.RANGE_METER = 100; W.DamageDropoff = "28-22";
		W.BulletVelocity = 700; W.SprintToFireTime = 0.3f;
		W.MOVEMENT_SPEED = 1.0f;
		Data.Add(EWeaponType::AssaultRifle, W);
	}

	// AK-47 - High damage, high recoil
	{
		FWeaponDataRow W;
		W.WeaponName = "AK-47"; W.WeaponType = EWeaponType::AssaultRifle;
		W.Damage = 36; W.HeadshotMultiplier = 1.4f; W.FireRate = 680;
		W.MagazineSize = 30; W.ReloadTime = 2.5f; W.ADSInTime = 0.28f;
		W.HIP_SPREAD = 5.0f; W.ADS_SPREAD = 1.5f;
		W.RECOIL_VERTICAL = 55; W.RECOIL_HORIZONTAL = 20;
		W.RANGE_METER = 120; W.DamageDropoff = "36-27";
		W.BulletVelocity = 680; W.SprintToFireTime = 0.32f;
		W.MOVEMENT_SPEED = 0.95f;
		Data.Add(EWeaponType::AssaultRifle, W);
	}

	// XM4 - BO6 style
	{
		FWeaponDataRow W;
		W.WeaponName = "XM4"; W.WeaponType = EWeaponType::AssaultRifle;
		W.Damage = 30; W.HeadshotMultiplier = 1.6f; W.FireRate = 730;
		W.MagazineSize = 30; W.ReloadTime = 2.1f; W.ADSInTime = 0.24f;
		W.HIP_SPREAD = 3.8f; W.ADS_SPREAD = 0.9f;
		W.RECOIL_VERTICAL = 28; W.RECOIL_HORIZONTAL = 8;
		W.RANGE_METER = 90; W.DamageDropoff = "30-23";
		W.BulletVelocity = 720; W.SprintToFireTime = 0.28f;
		Data.Add(EWeaponType::AssaultRifle, W);
	}

	// 9mm Pistol
	{
		FWeaponDataRow W;
		W.WeaponName = "9mm Pistol"; W.WeaponType = EWeaponType::Pistol;
		W.Damage = 22; W.HeadshotMultiplier = 1.8f; W.FireRate = 400;
		W.MagazineSize = 12; W.ReloadTime = 1.5f; W.ADSInTime = 0.15f;
		W.HIP_SPREAD = 5.0f; W.ADS_SPREAD = 1.5f;
		W.RECOIL_VERTICAL = 20; W.RECOIL_HORIZONTAL = 5;
		W.RANGE_METER = 50; W.DamageDropoff = "22-15";
		W.BulletVelocity = 400; W.SprintToFireTime = 0.15f;
		Data.Add(EWeaponType::Pistol, W);
	}

	// Heavy Pistol (.50 GS)
	{
		FWeaponDataRow W;
		W.WeaponName = ".50 GS"; W.WeaponType = EWeaponType::Pistol;
		W.Damage = 55; W.HeadshotMultiplier = 1.5f; W.FireRate = 180;
		W.MagazineSize = 7; W.ReloadTime = 2.0f; W.ADSInTime = 0.18f;
		W.HIP_SPREAD = 6.0f; W.ADS_SPREAD = 2.0f;
		W.RECOIL_VERTICAL = 80; W.RECOIL_HORIZONTAL = 15;
		W.RANGE_METER = 60; W.DamageDropoff = "55-35";
		W.BulletVelocity = 500; W.SprintToFireTime = 0.2f;
		Data.Add(EWeaponType::Pistol, W);
	}

	// Shotgun
	{
		FWeaponDataRow W;
		W.WeaponName = "Marine SP"; W.WeaponType = EWeaponType::Shotgun;
		W.Damage = 120; W.HeadshotMultiplier = 1.2f; W.FireRate = 80;
		W.MagazineSize = 8; W.ReloadTime = 3.5f; W.ADSInTime = 0.3f;
		W.HIP_SPREAD = 8.0f; W.ADS_SPREAD = 3.0f;
		W.RECOIL_VERTICAL = 120; W.RECOIL_HORIZONTAL = 20;
		W.RANGE_METER = 25; W.DamageDropoff = "120-40";
		W.BulletVelocity = 300; W.SprintToFireTime = 0.35f;
		Data.Add(EWeaponType::Shotgun, W);
	}

	// Sniper Rifle
	{
		FWeaponDataRow W;
		W.WeaponName = "SR-25"; W.WeaponType = EWeaponType::SniperRifle;
		W.Damage = 95; W.HeadshotMultiplier = 2.5f; W.FireRate = 90;
		W.MagazineSize = 10; W.ReloadTime = 3.0f; W.ADSInTime = 0.45f;
		W.HIP_SPREAD = 10.0f; W.ADS_SPREAD = 0.1f;
		W.RECOIL_VERTICAL = 100; W.RECOIL_HORIZONTAL = 15;
		W.RANGE_METER = 300; W.DamageDropoff = "95-70";
		W.BulletVelocity = 1200; W.SprintToFireTime = 0.4f;
		Data.Add(EWeaponType::SniperRifle, W);
	}

	// SMG - CQB
	{
		FWeaponDataRow W;
		W.WeaponName = "CQB-9"; W.WeaponType = EWeaponType::SMG;
		W.Damage = 24; W.HeadshotMultiplier = 1.7f; W.FireRate = 900;
		W.MagazineSize = 35; W.ReloadTime = 1.8f; W.ADSInTime = 0.2f;
		W.HIP_SPREAD = 3.5f; W.ADS_SPREAD = 1.2f;
		W.RECOIL_VERTICAL = 20; W.RECOIL_HORIZONTAL = 8;
		W.RANGE_METER = 65; W.DamageDropoff = "24-16";
		W.BulletVelocity = 550; W.SprintToFireTime = 0.2f;
		W.MOVEMENT_SPEED = 1.1f;
		Data.Add(EWeaponType::SMG, W);
	}

	return Data;
}
