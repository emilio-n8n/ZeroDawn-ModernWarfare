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

	UFUNCTION(BlueprintCallable, Category = "Battle Pass")
	void ClaimTierReward(int32 Tier);

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Battle Pass")
	void OnTierUnlocked(int32 Tier);

};
