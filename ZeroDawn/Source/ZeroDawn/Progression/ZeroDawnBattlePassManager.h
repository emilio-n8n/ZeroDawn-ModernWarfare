#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnBattlePassManager.generated.h"

USTRUCT(BlueprintType)
struct FBattlePassTier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TierNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XPRequired = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RewardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFreeReward = true;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZERODAWN_API UZeroDawnBattlePassManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnBattlePassManager();

	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	void AddBattlePassXP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	int32 GetCurrentTier() const { return CurrentTier; }

	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	int32 GetCurrentTierXP() const { return CurrentTierXP; }

	/**
	* Claim the reward for a given Battle Pass tier.
	* Validates tier bounds (1..MaxTier), checks if already claimed,
	* grants the reward, and marks the tier as claimed.
	* @param Tier - The tier number to claim (1-based)
	*/
	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	void ClaimTierReward(int32 Tier);

	/** Returns whether a given tier has been claimed. */
	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	bool IsTierClaimed(int32 Tier) const { return ClaimedTiers.Contains(Tier); }

	/** Returns the set of claimed tier numbers. */
	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	TSet<int32> GetClaimedTiers() const { return ClaimedTiers; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle Pass")
	TArray<FBattlePassTier> Tiers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Pass")
	int32 CurrentTier = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Pass")
	int32 CurrentTierXP = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Pass")
	int32 MaxTier = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Pass")
	bool bHasPremiumPass = false;

	/** Tracks which tier rewards have been claimed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Pass")
	TSet<int32> ClaimedTiers;

	/** Called when a tier reward is successfully claimed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle Pass")
	void OnRewardGranted(int32 Tier, const FString& RewardName);

	/** Helper to parse a reward name string and grant the appropriate reward. */
	void GrantRewardForName(const FString& RewardName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Battle Pass")
	void OnTierUnlocked(int32 Tier);

};
