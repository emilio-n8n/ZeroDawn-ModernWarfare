#pragma once
#include "ZeroDawnGameModeBase.h"
#include "ZeroDawnFFAGameMode.generated.h"

UCLASS()
class AZeroDawnFFAGameMode : public AZeroDawnGameModeBase
{
	GENERATED_BODY()

public:
	AZeroDawnFFAGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void CheckMatchEndConditions() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FFA")
	int32 FFAScoreLimit = 30;
};
