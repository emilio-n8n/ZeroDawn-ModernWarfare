#include "ZeroDawnBattlePassManager.h"

UZeroDawnBattlePassManager::UZeroDawnBattlePassManager()
{
	SetIsReplicatedByDefault(true);
	CurrentTier = 1;
	CurrentTierXP = 0;
	MaxTier = 100;
}

void UZeroDawnBattlePassManager::AddBattlePassXP(int32 Amount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	CurrentTierXP += Amount;

	while (CurrentTier < MaxTier)
	{
		int32 XPRequired = 10000;
		if (Tiers.IsValidIndex(CurrentTier - 1))
		{
			XPRequired = Tiers[CurrentTier - 1].XPRequired;
		}

		if (CurrentTierXP >= XPRequired)
		{
			CurrentTierXP -= XPRequired;
			CurrentTier++;
			OnTierUnlocked(CurrentTier);
		}
		else
		{
			break;
		}
	}
}

void UZeroDawnBattlePassManager::ClaimTierReward(int32 Tier)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
}
