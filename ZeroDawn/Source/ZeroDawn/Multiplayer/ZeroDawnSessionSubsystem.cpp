#include "ZeroDawnSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

// ============================================================
// Construction / Lifecycle
// ============================================================

UZeroDawnSessionSubsystem::UZeroDawnSessionSubsystem()
	: CurrentSessionName(NAME_GameSession)
{
}

void UZeroDawnSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Resolve the active online subsystem (Steam based on our project config).
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		SessionInterface = OnlineSub->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Register all session lifecycle callbacks so they fire into our BP-visible delegates.
			OnCreateSessionCompleteHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
				FOnCreateSessionCompleteDelegate::CreateUObject(this, &UZeroDawnSessionSubsystem::OnCreateSessionCompleted)
			);

			OnFindSessionsCompleteHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
				FOnFindSessionsCompleteDelegate::CreateUObject(this, &UZeroDawnSessionSubsystem::OnFindSessionsCompleted)
			);

			OnJoinSessionCompleteHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
				FOnJoinSessionCompleteDelegate::CreateUObject(this, &UZeroDawnSessionSubsystem::OnJoinSessionCompleted)
			);

			OnDestroySessionCompleteHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
				FOnDestroySessionCompleteDelegate::CreateUObject(this, &UZeroDawnSessionSubsystem::OnDestroySessionCompleted)
			);
		}
	}
}

void UZeroDawnSessionSubsystem::Deinitialize()
{
	// Unregister all callbacks before the subsystem is torn down.
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteHandle);
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);
	}

	Super::Deinitialize();
}

// ============================================================
// CreateSession
// ============================================================

void UZeroDawnSessionSubsystem::CreateSession(int32 NumPublicConnections, const FString& MapName, const FString& GameModeName)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSession failed: no session interface (OnlineSubsystem null or unavailable)."));
		OnCreateSessionComplete.Broadcast(false);
		return;
	}

	UWorld* World = GetGameInstance()->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSession failed: no world."));
		OnCreateSessionComplete.Broadcast(false);
		return;
	}

	ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
	if (!LocalPlayer || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSession failed: local player has no valid unique net ID."));
		OnCreateSessionComplete.Broadcast(false);
		return;
	}

	// Destroy any existing session before creating a new one.
	if (SessionInterface->GetNamedSession(CurrentSessionName) != nullptr)
	{
		SessionInterface->DestroySession(CurrentSessionName);
	}

	// ---- Build FOnlineSessionSettings ----
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch			= false;
	SessionSettings.bIsDedicated		= false;
	SessionSettings.bUsesPresence		= true;
	SessionSettings.bAllowJoinInProgress	= true;
	SessionSettings.bAllowJoinViaPresence	= true;
	SessionSettings.bShouldAdvertise	= true;
	SessionSettings.bUseLobbiesIfAvailable	= true;
	SessionSettings.NumPublicConnections	= NumPublicConnections;

	// Custom session metadata — these are published to the lobby/store so
	// searching clients can read them before joining.
	SessionSettings.Set(SETTING_GAMEMODE, GameModeName, EOnlineDataAdvertisementType::ViaOnlineService);
	SessionSettings.Set(SETTING_MAPNAME,  MapName,      EOnlineDataAdvertisementType::ViaOnlineService);

	bIsHost = true;

	SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), CurrentSessionName, SessionSettings);
}

// ============================================================
// FindSessions
// ============================================================

void UZeroDawnSessionSubsystem::FindSessions()
{
	if (!SessionInterface.IsValid())
	{
		OnFindSessionsComplete.Broadcast(false);
		return;
	}

	UWorld* World = GetGameInstance()->GetWorld();
	if (!World)
	{
		OnFindSessionsComplete.Broadcast(false);
		return;
	}

	ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
	if (!LocalPlayer || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
	{
		OnFindSessionsComplete.Broadcast(false);
		return;
	}

	// Create a search object that looks for presence-enabled sessions.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery		= false;
	SessionSearch->MaxSearchResults	= 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

// ============================================================
// JoinSession
// ============================================================

void UZeroDawnSessionSubsystem::JoinSession(int32 SessionIndex)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		OnJoinSessionComplete.Broadcast(false);
		return;
	}

	if (!SessionSearch->SearchResults.IsValidIndex(SessionIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinSession failed: index %d out of range (have %d results)."),
			SessionIndex, SessionSearch->SearchResults.Num());
		OnJoinSessionComplete.Broadcast(false);
		return;
	}

	UWorld* World = GetGameInstance()->GetWorld();
	if (!World)
	{
		OnJoinSessionComplete.Broadcast(false);
		return;
	}

	ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
	if (!LocalPlayer || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
	{
		OnJoinSessionComplete.Broadcast(false);
		return;
	}

	bIsHost = false;

	SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), CurrentSessionName, SessionSearch->SearchResults[SessionIndex]);
}

// ============================================================
// DestroySession
// ============================================================

void UZeroDawnSessionSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		OnDestroySessionComplete.Broadcast(false);
		return;
	}

	if (SessionInterface->GetNamedSession(CurrentSessionName) == nullptr)
	{
		// Nothing to destroy — broadcast success immediately.
		OnDestroySessionComplete.Broadcast(true);
		return;
	}

	bIsHost = false;

	SessionInterface->DestroySession(CurrentSessionName);
}

// ============================================================
// Query Helpers
// ============================================================

int32 UZeroDawnSessionSubsystem::GetSearchResultCount() const
{
	return SessionSearch.IsValid() ? SessionSearch->SearchResults.Num() : 0;
}

bool UZeroDawnSessionSubsystem::GetSearchResultInfo(int32 Index, FString& OutServerName, FString& OutMapName,
	FString& OutGameMode, int32& OutCurrentPlayers, int32& OutMaxPlayers, int32& OutPing) const
{
	if (!SessionSearch.IsValid() || !SessionSearch->SearchResults.IsValidIndex(Index))
	{
		return false;
	}

	const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[Index];
	const FOnlineSession& Session = Result.Session;

	// Optional: SETTING_SERVERNAME may not be set; fall back to a default.
	FString ServerName;
	Session.SessionSettings.Get(SETTING_SERVERNAME, ServerName);
	OutServerName = ServerName.IsEmpty() ? TEXT("Game Session") : ServerName;

	Session.SessionSettings.Get(SETTING_GAMEMODE, OutGameMode);
	Session.SessionSettings.Get(SETTING_MAPNAME, OutMapName);

	OutCurrentPlayers = Session.SessionSettings.NumPublicConnections - Session.NumOpenPublicConnections;
	OutMaxPlayers	  = Session.SessionSettings.NumPublicConnections;
	OutPing			  = Result.PingInMs;

	return true;
}

// ============================================================
// Internal Callbacks
// ============================================================

void UZeroDawnSessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	// Only broadcast if this callback matches our active session.
	if (SessionName == CurrentSessionName)
	{
		bIsHost = bWasSuccessful;
		OnCreateSessionComplete.Broadcast(bWasSuccessful);
	}
}

void UZeroDawnSessionSubsystem::OnFindSessionsCompleted(bool bWasSuccessful)
{
	OnFindSessionsComplete.Broadcast(bWasSuccessful);
}

void UZeroDawnSessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionName == CurrentSessionName)
	{
		const bool bWasSuccessful = (Result == EOnJoinSessionCompleteResult::Success);
		OnJoinSessionComplete.Broadcast(bWasSuccessful);
	}
}

void UZeroDawnSessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (SessionName == CurrentSessionName)
	{
		OnDestroySessionComplete.Broadcast(bWasSuccessful);
	}
}
