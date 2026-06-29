#pragma once
#include "ZeroDawn.h"
#include "GameTypes.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None UMETA(DisplayName = "None"),
	Spectator UMETA(DisplayName = "Spectator"),
	Alpha UMETA(DisplayName = "Alpha Team"),
	Bravo UMETA(DisplayName = "Bravo Team")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	Pistol UMETA(DisplayName = "Pistol"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	SMG UMETA(DisplayName = "SMG"),
	Knife UMETA(DisplayName = "Knife"),
	Grenade UMETA(DisplayName = "Grenade")
};

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Single UMETA(DisplayName = "Single"),
	Burst UMETA(DisplayName = "Burst"),
	Automatic UMETA(DisplayName = "Automatic")
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Alert UMETA(DisplayName = "Alert"),
	Combat UMETA(DisplayName = "Combat"),
	Search UMETA(DisplayName = "Search"),
	Flee UMETA(DisplayName = "Flee"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EKillstreakType : uint8
{
	UAV UMETA(DisplayName = "UAV"),
	CarePackage UMETA(DisplayName = "Care Package"),
	AttackHelicopter UMETA(DisplayName = "Attack Helicopter"),
	Gunship UMETA(DisplayName = "Gunship"),
	Nuke UMETA(DisplayName = "Nuke")
};

UENUM(BlueprintType)
enum class EPerkSlot : uint8
{
	Perk1 UMETA(DisplayName = "Perk Slot 1"),
	Perk2 UMETA(DisplayName = "Perk Slot 2"),
	Perk3 UMETA(DisplayName = "Perk Slot 3")
};

UENUM(BlueprintType)
enum class EPerkType : uint8
{
	FlakJacket UMETA(DisplayName = "Flak Jacket"),
	EOD UMETA(DisplayName = "EOD"),
	DoubleTime UMETA(DisplayName = "Double Time"),
	ColdBlooded UMETA(DisplayName = "Cold Blooded"),
	Hardline UMETA(DisplayName = "Hardline"),
	QuickFix UMETA(DisplayName = "Quick Fix"),
	Amped UMETA(DisplayName = "Amped"),
	Nimble UMETA(DisplayName = "Nimble"),
	Ghost UMETA(DisplayName = "Ghost"),
	Overkill UMETA(DisplayName = "Overkill")
};

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	TeamDeathmatch UMETA(DisplayName = "Team Deathmatch"),
	FreeForAll UMETA(DisplayName = "Free For All"),
	Domination UMETA(DisplayName = "Domination"),
	SearchAndDestroy UMETA(DisplayName = "Search & Destroy"),
	Hardpoint UMETA(DisplayName = "Hardpoint")
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	WaitingForPlayers UMETA(DisplayName = "Waiting For Players"),
	PreMatch UMETA(DisplayName = "Pre Match"),
	InProgress UMETA(DisplayName = "In Progress"),
	Halftime UMETA(DisplayName = "Halftime"),
	MatchEnded UMETA(DisplayName = "Match Ended")
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::AssaultRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float HeadshotMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineSize = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AimDownSightTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Spread = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float SpreadADS = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RecoilVertical = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RecoilHorizontal = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsAutomatic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 BurstCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BulletSpeed = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DamageFalloffStart = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DamageFalloffEnd = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DamageFalloffMultiplier = 0.5f;
};

USTRUCT(BlueprintType)
struct FPlayerScoreData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Kills = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Deaths = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Assists = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Ping = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	ETeamType Team = ETeamType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Killstreak = 0;
};

USTRUCT(BlueprintType)
struct FWeaponAttachment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	FName AttachmentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	float RecoilReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	float SpreadReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	float AimSpeedBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	float DamageRangeBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	float ReloadSpeedBonus = 0.0f;
};

USTRUCT(BlueprintType)
struct FLoadoutClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	FString ClassName = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	EWeaponType PrimaryWeapon = EWeaponType::AssaultRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	EWeaponType SecondaryWeapon = EWeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	EPerkType Perk1 = EPerkType::FlakJacket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	EPerkType Perk2 = EPerkType::ColdBlooded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	EPerkType Perk3 = EPerkType::Amped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TArray<FWeaponAttachment> Attachments;
};
