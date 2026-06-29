#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnGameModeBase.generated.h"

class AZeroDawnCharacter;
class AZeroDawnPlayerState;
class AZeroDawnGameState;

UCLASS()
class AZeroDawnGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	AZeroDawnGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	EGameModeType GameModeType = EGameModeType::TeamDeathmatch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 ScoreLimit = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float MatchTimeLimit = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float PreMatchTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float RespawnDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 MaxPlayers = 12;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ScoreKill(AController* Killer, AController* Victim, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ScoreTeamKill(AController* Killer);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RespawnPlayer(AController* Player);

	bool IsMatchActive() const;

	void EndCurrentMatch();

protected:
	AZeroDawnGameState* GetZDGameState() const;
	AZeroDawnCharacter* SpawnDefaultPawnForController(AController* NewPlayer);

	void StartPreMatchCountdown();
	void StartMatch();
	virtual void CheckMatchEndConditions();

	FTimerHandle PreMatchTimerHandle;
	FTimerHandle MatchTimerHandle;

	float MatchStartTime = 0.0f;

	virtual void OnPlayerKilled(AController* Killer, AController* Victim);
	virtual void OnMatchEnd();

	UPROPERTY()
	TArray<AActor*> AlphaSpawnPoints;

	UPROPERTY()
	TArray<AActor*> BravoSpawnPoints;

	UPROPERTY()
	TArray<AActor*> NeutralSpawnPoints;

	void FindSpawnPoints();
};
