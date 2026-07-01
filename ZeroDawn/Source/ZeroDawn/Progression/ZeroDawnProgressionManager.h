#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnProgressionManager.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XPRequired = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EWeaponType> UnlockedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPerkType> UnlockedPerks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> UnlockedAttachments;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZERODAWN_API UZeroDawnProgressionManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnProgressionManager();

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddXP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	int32 GetCurrentLevel() const { return CurrentLevel; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	int32 GetCurrentXP() const { return CurrentXP; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	int32 GetXPForNextLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void SetLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	TArray<EWeaponType> GetUnlockedWeapons() const;

	UFUNCTION(BlueprintCallable, Category = "Progression")
	TArray<EPerkType> GetUnlockedPerks() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
	TArray<FLevelData> LevelData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 CurrentLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 CurrentXP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
	int32 MaxLevel = 55;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
	float XPPerKill = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
	float XPPerWin = 500.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Progression")
	void OnLevelUp(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void TriggerSaveOnLevelUp();

protected:
	void CheckLevelUp();
};
