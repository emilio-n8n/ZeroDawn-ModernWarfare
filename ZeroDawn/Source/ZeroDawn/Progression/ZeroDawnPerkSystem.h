#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnPerkSystem.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnPerkSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnPerkSystem();

	UFUNCTION(BlueprintCallable, Category = "Perks")
	void ApplyPerks(const FLoadoutClass& Loadout);

	UFUNCTION(BlueprintCallable, Category = "Perks")
	bool HasPerk(EPerkType Perk) const;

	UFUNCTION(BlueprintCallable, Category = "Perks")
	float GetPerkModifiedValue(float BaseValue, EPerkType RelevantPerk) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perks")
	TArray<EPerkType> ActivePerks;

	UFUNCTION(BlueprintCallable, Category = "Perks")
	void ApplyPerkEffect(EPerkType Perk);

protected:
	void ApplyFlakJacket();
	void ApplyEOD();
	void ApplyDoubleTime();
	void ApplyColdBlooded();
	void ApplyHardline();
	void ApplyQuickFix();
	void ApplyAmped();
	void ApplyNimble();
	void ApplyGhost();
	void ApplyOverkill();
};
