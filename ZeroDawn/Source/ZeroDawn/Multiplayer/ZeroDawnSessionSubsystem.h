#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnSessionSubsystem.generated.h"

// ============================================================
// Blueprint-assignable delegates for session lifecycle events
// ============================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionCompleteBP, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindSessionsCompleteBP, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinSessionCompleteBP, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySessionCompleteBP, bool, bSuccessful);

/**
 * UZeroDawnSessionSubsystem
 *
 * Manages Steam (or other online platform) sessions for Zero Dawn multiplayer.
 * Provides CreateSession, FindSessions, JoinSession, DestroySession via IOnlineSession.
 * FSessionSettings includes game mode and map name for session advertising.
 */
UCLASS()
class UZeroDawnSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UZeroDawnSessionSubsystem();

	// ============================================================
	// Session Operations (BlueprintCallable)
	// ============================================================

	/**
	 * Create a new online session.
	 * @param NumPublicConnections - Max number of players (e.g., 12 for 6v6, 2 for 1v1)
	 * @param MapName              - Name of the map being played (stored in settings)
	 * @param GameModeName         - Name of the game mode (e.g., "TeamDeathmatch")
	 */
	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void CreateSession(int32 NumPublicConnections, const FString& MapName, const FString& GameModeName);

	/**
	 * Search for available online sessions.
	 * Results are available via GetSearchResultCount() / GetSearchResultInfo().
	 */
	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void FindSessions();

	/**
	 * Join an online session discovered via FindSessions.
	 * @param SessionIndex - Index into the cached search results array
	 */
	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void JoinSession(int32 SessionIndex);

	/**
	 * Destroy the current online session (host only).
	 */
	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void DestroySession();

	// ============================================================
	// Delegates (BlueprintAssignable)
	// ============================================================

	UPROPERTY(BlueprintAssignable, Category = "Online|Sessions")
	FOnCreateSessionCompleteBP OnCreateSessionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Online|Sessions")
	FOnFindSessionsCompleteBP OnFindSessionsComplete;

	UPROPERTY(BlueprintAssignable, Category = "Online|Sessions")
	FOnJoinSessionCompleteBP OnJoinSessionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Online|Sessions")
	FOnDestroySessionCompleteBP OnDestroySessionComplete;

	// ============================================================
	// Query Helpers (BlueprintPure / Callable)
	// ============================================================

	/** Number of sessions found in the last search. */
	UFUNCTION(BlueprintPure, Category = "Online|Sessions")
	int32 GetSearchResultCount() const;

	/**
	 * Get detailed info about a search result.
	 * @param Index           - Index into the cached results
	 * @param OutServerName   - Display name of the server/game
	 * @param OutMapName      - Map being played
	 * @param OutGameMode     - Game mode being played
	 * @param OutCurrentPlayers - Current number of players
	 * @param OutMaxPlayers   - Max players allowed
	 * @param OutPing         - Ping in ms
	 * @return true if the index was valid and data was retrieved
	 */
	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	bool GetSearchResultInfo(int32 Index, FString& OutServerName, FString& OutMapName,
		FString& OutGameMode, int32& OutCurrentPlayers, int32& OutMaxPlayers, int32& OutPing) const;

	/** Whether this local player created/hosted the current session. */
	UFUNCTION(BlueprintPure, Category = "Online|Sessions")
	bool IsHost() const { return bIsHost; }

protected:
	// UGameInstanceSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	// ============================================================
	// Internal Callbacks
	// ============================================================

	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsCompleted(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);

	// ============================================================
	// Session Interface
	// ============================================================

	/** Cached pointer to the online session interface (Steam via IOnlineSubsystem). */
	IOnlineSessionPtr SessionInterface;

	/** Delegate handles for cleanup in Deinitialize. */
	FDelegateHandle OnCreateSessionCompleteHandle;
	FDelegateHandle OnFindSessionsCompleteHandle;
	FDelegateHandle OnJoinSessionCompleteHandle;
	FDelegateHandle OnDestroySessionCompleteHandle;

	/** The name used for the current session (typically NAME_GameSession). */
	FName CurrentSessionName;

	/** Search object used during FindSessions; stores results. */
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	/** Whether this player created the current session. */
	bool bIsHost = false;
};
