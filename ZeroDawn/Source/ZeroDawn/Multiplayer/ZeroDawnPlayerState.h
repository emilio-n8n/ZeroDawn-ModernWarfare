#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnPlayerState.generated.h"

class UZeroDawnSaveGame;

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

	/** Server RPC to toggle ready state from the client (used in lobby widget). */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetReady(bool bNewReady);

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

	// ============================================================
	// Save / Load System
	// ============================================================

	/**
	* Server-initiated: tells this player's client to save their game data.
	* Called on level up, match end, settings changes, and loadout changes.
	*/
	UFUNCTION(Client, Reliable)
	void ClientSaveGameData();

	/**
	* Client-side: collects all current data and persists to disk via UGameplayStatics.
	* Called when this PlayerState receives the ClientSaveGameData RPC.
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveCurrentProgress();

	/**
	* Client-side: loads saved data from disk and restores progression.
	* Called automatically in BeginPlay() on the client.
	* Can also be called manually to refresh data.
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadSavedProgress();

	/**
	* Get a player identifier string suitable for save slot naming.
	* Uses unique net ID if available, otherwise falls back to player name or a default.
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FString GetSavePlayerId() const;

	/**
	* Track a win/loss for career stats (called on match end).
	*/
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddWin();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddLoss();

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 CareerWins = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Score")
	int32 CareerLosses = 0;

	/**
	* Track a weapon kill for usage statistics.
	* @param WeaponType - The type of weapon used for this kill
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void RecordWeaponKill(EWeaponType WeaponType);

	/** Map of weapon type → kill count for most-used weapon tracking */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "SaveGame")
	TMap<EWeaponType, int32> WeaponUsageMap;
};
