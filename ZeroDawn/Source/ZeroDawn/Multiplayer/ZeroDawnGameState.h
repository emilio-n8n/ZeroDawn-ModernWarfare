#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnGameState.generated.h"

UCLASS()
class AZeroDawnGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AZeroDawnGameState();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	EMatchState MatchState = EMatchState::WaitingForPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	EGameModeType CurrentGameMode = EGameModeType::TeamDeathmatch;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	float MatchTime = 0.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	float MaxMatchTime = 600.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	int32 ScoreLimit = 100;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	int32 TeamAlphaScore = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	int32 TeamBravoScore = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	TArray<FPlayerScoreData> PlayerScoreboard;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	float PreMatchCountdown = 10.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	float RemainingPreMatchTime = 10.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	ETeamType WinningTeam = ETeamType::None;

	UFUNCTION(BlueprintCallable, Category = "Match")
	void AddTeamScore(ETeamType Team, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Match")
	void SetMatchState(EMatchState NewState);

	UFUNCTION(BlueprintCallable, Category = "Match")
	void UpdateScoreboard();

	UFUNCTION(BlueprintCallable, Category = "Match")
	FString GetMatchTimeFormatted() const;

	UFUNCTION(BlueprintCallable, Category = "Match")
	int32 GetPlayerCount() const;

	UFUNCTION(BlueprintCallable, Category = "Match")
	int32 GetMaxPlayers() const { return 12; }
};
