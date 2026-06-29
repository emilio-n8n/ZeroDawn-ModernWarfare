#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnPlayerState.generated.h"

UCLASS()
class AZeroDawnPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AZeroDawnPlayerState();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddKill();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddDeath();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddAssist();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScorePoints(int32 Amount);

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 TotalKills = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 TotalDeaths = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 TotalAssists = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 TotalScore = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 CurrentKillstreak = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 HighestKillstreak = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 PlayerLevel = 1;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 PlayerXP = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Team")
	ETeamType PlayerTeam = ETeamType::None;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	bool bIsReady = false;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Match")
	bool bHasSpawned = false;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Loadout")
	FLoadoutClass CurrentLoadout;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void RequestRespawn();

	UFUNCTION(Client, Reliable)
	void ClientRequestRespawn();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	FPlayerScoreData GetScoreData() const;
};
