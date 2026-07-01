#pragma once
#include "ZeroDawnGameModeBase.h"
#include "ZeroDawnHardpointGameMode.generated.h"

class AHardpointZone;

UCLASS()
class AZeroDawnHardpointGameMode : public AZeroDawnGameModeBase
{
	GENERATED_BODY()

public:
	AZeroDawnHardpointGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void CheckMatchEndConditions() override;
	virtual void HandleMatchHasEnded() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hardpoint")
	int32 HardpointScoreLimit = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hardpoint")
	float HardpointRotationTime = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hardpoint")
	int32 PointsPerSecond = 1;

	TArray<AHardpointZone*> AllHardpoints;
	int32 CurrentHardpointIndex = 0;

	float LastScoreTime = 0.0f;
	float HardpointActiveTime = 0.0f;

	void ActivateNextHardpoint();
};
