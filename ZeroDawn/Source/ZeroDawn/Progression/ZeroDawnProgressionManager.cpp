#include "ZeroDawnProgressionManager.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"

UZeroDawnProgressionManager::UZeroDawnProgressionManager()
{
	SetIsReplicatedByDefault(true);
	CurrentLevel = 1;
	CurrentXP = 0;
}

void UZeroDawnProgressionManager::AddXP(int32 Amount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	CurrentXP += Amount;
	CheckLevelUp();
}

void UZeroDawnProgressionManager::SetLevel(int32 NewLevel)
{
	CurrentLevel = FMath::Clamp(NewLevel, 1, MaxLevel);
	CurrentXP = 0;
	OnLevelUp(CurrentLevel);
	TriggerSaveOnLevelUp();
}

void UZeroDawnProgressionManager::CheckLevelUp()
{
	bool bLeveledUp = false;
	while (CurrentLevel < MaxLevel)
	{
		int32 XPRequired = GetXPForNextLevel();
		if (CurrentXP >= XPRequired)
		{
			CurrentXP -= XPRequired;
			CurrentLevel++;
			OnLevelUp(CurrentLevel);
			bLeveledUp = true;
		}
		else
		{
			break;
		}
	}

	// Save progress after any level-ups
	if (bLeveledUp)
	{
		TriggerSaveOnLevelUp();
	}
}

void UZeroDawnProgressionManager::TriggerSaveOnLevelUp()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->HasAuthority()) return;

	AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OwnerActor);
	if (!Char) return;

	AZeroDawnPlayerState* PS = Char->GetPlayerState<AZeroDawnPlayerState>();
	if (PS)
	{
		PS->ClientSaveGameData();
	}
}

int32 UZeroDawnProgressionManager::GetXPForNextLevel() const
{
	if (LevelData.IsValidIndex(CurrentLevel - 1))
	{
		return LevelData[CurrentLevel - 1].XPRequired;
	}
	return CurrentLevel * 1000;
}

TArray<EWeaponType> UZeroDawnProgressionManager::GetUnlockedWeapons() const
{
	TArray<EWeaponType> Unlocked;
	for (int32 i = 0; i < CurrentLevel && i < LevelData.Num(); i++)
	{
		Unlocked.Append(LevelData[i].UnlockedWeapons);
	}
	return Unlocked;
}

TArray<EPerkType> UZeroDawnProgressionManager::GetUnlockedPerks() const
{
	TArray<EPerkType> Unlocked;
	for (int32 i = 0; i < CurrentLevel && i < LevelData.Num(); i++)
	{
		Unlocked.Append(LevelData[i].UnlockedPerks);
	}
	return Unlocked;
}
