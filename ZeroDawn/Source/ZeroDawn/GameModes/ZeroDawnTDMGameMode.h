#pragma once
#include "ZeroDawnGameModeBase.h"
#include "ZeroDawnTDMGameMode.generated.h"

UCLASS()
class AZeroDawnTDMGameMode : public AZeroDawnGameModeBase
{
	GENERATED_BODY()

public:
	AZeroDawnTDMGameMode();

	virtual void CheckMatchEndConditions() override;
	virtual void OnPlayerKilled(AController* Killer, AController* Victim) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TDM")
	int32 TDMScoreLimit = 100;
};
