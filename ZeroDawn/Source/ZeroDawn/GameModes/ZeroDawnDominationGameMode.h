#pragma once
#include "ZeroDawnGameModeBase.h"
#include "ZeroDawnDominationGameMode.generated.h"

class ADominationZone;

UCLASS()
class AZeroDawnDominationGameMode : public AZeroDawnGameModeBase
{
	GENERATED_BODY()

public:
	AZeroDawnDominationGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void CheckMatchEndConditions() override;
	virtual void HandleMatchHasEnded() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Domination")
	int32 DominationScoreLimit = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Domination")
	float ScoreInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Domination")
	int32 PointsPerTick = 1;

	float LastScoreTime = 0.0f;
};
