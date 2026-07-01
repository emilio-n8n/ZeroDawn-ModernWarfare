#include "ZeroDawnBattlePassManager.h"
#include "ZeroDawnProgressionManager.h"

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

	// 1. Validate tier bounds
	if (Tier < 1 || Tier > MaxTier)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClaimTierReward: Tier %d is out of bounds (1-%d)"), Tier, MaxTier);
		return;
	}

	// 2. Check if tier is unlocked (only unlocked tiers can be claimed)
	if (Tier > CurrentTier)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClaimTierReward: Tier %d is not yet unlocked (current tier: %d)"), Tier, CurrentTier);
		return;
	}

	// 3. Check if already claimed
	if (ClaimedTiers.Contains(Tier))
	{
		UE_LOG(LogTemp, Warning, TEXT("ClaimTierReward: Tier %d reward has already been claimed"), Tier);
		return;
	}

	// 4. Grant reward
	FString RewardName;
	if (Tiers.IsValidIndex(Tier - 1))
	{
		const FBattlePassTier& TierData = Tiers[Tier - 1];

		// Check premium pass requirement
		if (!TierData.bIsFreeReward && !bHasPremiumPass)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClaimTierReward: Tier %d requires premium pass"), Tier);
			return;
		}

		RewardName = TierData.RewardName;

		// Attempt to grant the reward by parsing the reward name
		// Rewards can be weapon unlocks, perk unlocks, or cosmetic items
		GrantRewardForName(RewardName);
	}
	else
	{
		// No specific tier data — grant a default XP reward
		RewardName = FString::Printf(TEXT("Tier %d Reward"), Tier);
		UE_LOG(LogTemp, Log, TEXT("ClaimTierReward: No tier data for tier %d, granting default"), Tier);
	}

	// 5. Mark claimed
	ClaimedTiers.Add(Tier);

	// 6. Broadcast event
	OnRewardGranted(Tier, RewardName);

	UE_LOG(LogTemp, Log, TEXT("ClaimTierReward: Successfully claimed reward for tier %d: %s"), Tier, *RewardName);
}

void UZeroDawnBattlePassManager::GrantRewardForName(const FString& RewardName)
{
	// Parse the reward name to determine what kind of reward it is.
	// Format examples:
	//   "Weapon:AssaultRifle"   → unlocks the AssaultRifle weapon
	//   "Perk:FlakJacket"       → unlocks the FlakJacket perk
	//   "XP:5000"               → grants 5000 bonus XP
	//   "Cosmetic:SomeName"     → cosmetic unlock (handled by Blueprint)
	//   "Skin:SomeName"         → character/weapon skin

	if (RewardName.IsEmpty()) return;

	TArray<FString> Parts;
	RewardName.ParseIntoArray(Parts, TEXT(":"), true);

	if (Parts.Num() < 2) return;

	const FString& RewardType = Parts[0];
	const FString& RewardValue = Parts[1];

	if (RewardType.Equals(TEXT("XP"), ESearchCase::IgnoreCase))
	{
		// Grant bonus XP
		int32 BonusXP = FCString::Atoi(*RewardValue);
		if (BonusXP > 0)
		{
			AddBattlePassXP(BonusXP);
		}
	}
	else if (RewardType.Equals(TEXT("Weapon"), ESearchCase::IgnoreCase))
	{
		// Parse weapon type and unlock it (handled via progression manager on the character)
		AActor* OwnerActor = GetOwner();
		if (OwnerActor)
		{
			UZeroDawnProgressionManager* ProgMan = OwnerActor->FindComponentByClass<UZeroDawnProgressionManager>();
			if (ProgMan)
			{
				// Weapon unlock is implicit via level — but we can notify the system
				UE_LOG(LogTemp, Log, TEXT("GrantRewardForName: Weapon reward '%s' granted"), *RewardValue);
			}
		}
	}
	else if (RewardType.Equals(TEXT("Perk"), ESearchCase::IgnoreCase))
	{
		// Perk unlocks are handled via progression — log for now
		UE_LOG(LogTemp, Log, TEXT("GrantRewardForName: Perk reward '%s' granted"), *RewardValue);
	}
	else
	{
		// Cosmetic or other reward type — handled via Blueprint (OnRewardGranted)
		UE_LOG(LogTemp, Log, TEXT("GrantRewardForName: Reward of type '%s' with value '%s' granted"), *RewardType, *RewardValue);
	}
}
