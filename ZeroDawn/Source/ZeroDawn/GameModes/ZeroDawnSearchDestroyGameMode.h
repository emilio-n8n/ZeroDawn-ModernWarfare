#pragma once
#include "ZeroDawnGameModeBase.h"
#include "ZeroDawnSearchDestroyGameMode.generated.h"

class AZeroDawnBomb;

UCLASS()
class ZERODAWN_API AZeroDawnSearchDestroyGameMode : public AZeroDawnGameModeBase
{
	GENERATED_BODY()

public:
	AZeroDawnSearchDestroyGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnPlayerKilled(AController* Killer, AController* Victim) override;
	virtual void CheckMatchEndConditions() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S&D")
	int32 RoundsToWin = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S&D")
	float BombTimer = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S&D")
	float PlantTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S&D")
	float DefuseTime = 7.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S&D")
	int32 AlphaRoundWins = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S&D")
	int32 BravoRoundWins = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S&D")
	int32 CurrentRound = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S&D")
	bool bBombPlanted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S&D")
	bool bRoundActive = false;

	/** Reference to the active bomb in the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S&D")
	AZeroDawnBomb* ActiveBomb = nullptr;

	UFUNCTION(BlueprintCallable, Category = "S&D")
	void StartNewRound();

	UFUNCTION(BlueprintCallable, Category = "S&D")
	void EndRound(ETeamType WinningTeam);

	UFUNCTION(BlueprintCallable, Category = "S&D")
	void OnBombPlanted();

	UFUNCTION(BlueprintCallable, Category = "S&D")
	void OnBombDefused();

	UFUNCTION(BlueprintCallable, Category = "S&D")
	void OnBombExploded();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S&D")
	float RoundTimeLimit = 120.0f;

	bool bSwitchedSides = false;
	float RoundStartTime = 0.0f;
	float BombPlantTime = 0.0f;
	int32 AlphaAlive = 0;
	int32 BravoAlive = 0;

	void CountAlivePlayers();

	/** Find and cache the bomb actor, bind its delegates */
	void FindAndBindBomb();
};
