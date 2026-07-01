#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnSaveGame.generated.h"

/**
* UZeroDawnSaveGame - Persistent save data for Zero Dawn: Modern Warfare.
* Stores progression (level, XP), loadouts (5 slots), settings (graphics, audio, controls),
* career stats (kills, deaths, wins, losses, playtime), and weapon usage tracking.
* 
* Save/Load is handled via UGameplayStatics::SaveGameToSlot / LoadGameFromSlot
* with a player-specific slot name for per-player persistence.
*/
UCLASS(BlueprintType)
class ZERODAWN_API UZeroDawnSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UZeroDawnSaveGame();

	// ============================================================
	// Progression Data
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 PlayerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 PlayerXP;

	// ============================================================
	// Loadouts (5 slots)
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Loadouts")
	TArray<FLoadoutClass> SavedLoadouts;

	// ============================================================
	// Settings - Graphics
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 GraphicsQuality;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ResolutionScale;

	// ============================================================
	// Settings - Audio
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	float MasterVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	float SFXVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	float MusicVolume;

	// ============================================================
	// Settings - Controls
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	float MouseSensitivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	float ADSMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bInvertY;

	// ============================================================
	// Career Stats
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CareerKills;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CareerDeaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CareerWins;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CareerLosses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float CareerPlaytime;

	// ============================================================
	// Weapon Usage Tracking (most-used weapon tally)
	// ============================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EWeaponType, int32> WeaponUsageMap;

	// ============================================================
	// Static Save/Load Helpers
	// ============================================================

	/**
	* Save game data to the player-specific slot.
	* @param WorldContext - Any UObject in the world for UGameplayStatics context
	* @param PlayerId - Unique identifier for the player (used in slot name)
	* @param SaveData - The fully populated save game instance to persist
	* @return true if the save succeeded
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	static bool SaveProgress(UObject* WorldContext, const FString& PlayerId, UZeroDawnSaveGame* SaveData);

	/**
	* Load game data from the player-specific slot.
	* @param WorldContext - Any UObject in the world for UGameplayStatics context
	* @param PlayerId - Unique identifier for the player (used in slot name)
	* @return The loaded save game data, or nullptr if no save exists
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	static UZeroDawnSaveGame* LoadProgress(const UObject* WorldContext, const FString& PlayerId);

	/**
	* Get the save slot name for a given player ID.
	* Format: "ZeroDawnSave_{PlayerId}"
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	static FString GetSlotName(const FString& PlayerId);
};
