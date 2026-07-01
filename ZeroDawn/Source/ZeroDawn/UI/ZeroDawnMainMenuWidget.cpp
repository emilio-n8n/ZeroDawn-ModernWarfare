#include "ZeroDawnMainMenuWidget.h"
#include "../Multiplayer/ZeroDawnSessionSubsystem.h"
#include "Components/Button.h"

// ============================================================
// Lifecycle
// ============================================================

void UZeroDawnMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Cache the session subsystem from the game instance.
	if (UGameInstance* GI = GetGameInstance())
	{
		SessionSubsystem = GI->GetSubsystem<UZeroDawnSessionSubsystem>();
	}

	// Bind button click events.
	if (HostGameButton)
	{
		HostGameButton->OnClicked.AddDynamic(this, &UZeroDawnMainMenuWidget::OnHostGameClicked);
	}

	if (FindGameButton)
	{
		FindGameButton->OnClicked.AddDynamic(this, &UZeroDawnMainMenuWidget::OnFindGameClicked);
	}

	// Register for session lifecycle callbacks.
	if (UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get())
	{
		Subsystem->OnCreateSessionComplete.AddDynamic(this, &UZeroDawnMainMenuWidget::OnCreateSessionCompleteBP);
		Subsystem->OnFindSessionsComplete.AddDynamic(this, &UZeroDawnMainMenuWidget::OnFindSessionsCompleteBP);
	}

	SetStatus(TEXT("Welcome to Zero Dawn. Host a game or find one to join."));
}

void UZeroDawnMainMenuWidget::NativeDestruct()
{
	// Unregister from session callbacks to avoid dangling delegates.
	if (UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get())
	{
		Subsystem->OnCreateSessionComplete.RemoveDynamic(this, &UZeroDawnMainMenuWidget::OnCreateSessionCompleteBP);
		Subsystem->OnFindSessionsComplete.RemoveDynamic(this, &UZeroDawnMainMenuWidget::OnFindSessionsCompleteBP);
	}

	Super::NativeDestruct();
}

// ============================================================
// Button Handlers
// ============================================================

void UZeroDawnMainMenuWidget::OnHostGameClicked()
{
	UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get();
	if (!Subsystem)
	{
		SetStatus(TEXT("Error: Session subsystem not available."));
		return;
	}

	SetStatus(TEXT("Creating session..."));

	// Create a public online session with the configured defaults.
	Subsystem->CreateSession(MaxPlayers, DefaultMapName, DefaultGameMode);
}

void UZeroDawnMainMenuWidget::OnFindGameClicked()
{
	UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get();
	if (!Subsystem)
	{
		SetStatus(TEXT("Error: Session subsystem not available."));
		return;
	}

	SetStatus(TEXT("Searching for sessions..."));

	// Clear any previously shown results.
	if (SessionResultsBox)
	{
		SessionResultsBox->ClearChildren();
	}

	Subsystem->FindSessions();
}

void UZeroDawnMainMenuWidget::OnJoinSessionClicked(int32 SessionIndex)
{
	UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get();
	if (!Subsystem)
	{
		SetStatus(TEXT("Error: Session subsystem not available."));
		return;
	}

	SetStatus(FString::Printf(TEXT("Joining session %d..."), SessionIndex));
	Subsystem->JoinSession(SessionIndex);
}

// ============================================================
// Session Callbacks
// ============================================================

void UZeroDawnMainMenuWidget::OnCreateSessionCompleteBP(bool bSuccessful)
{
	if (bSuccessful)
	{
		SetStatus(TEXT("Session created! Entering lobby..."));
	}
	else
	{
		SetStatus(TEXT("Failed to create session. Please try again."));
	}
}

void UZeroDawnMainMenuWidget::OnFindSessionsCompleteBP(bool bSuccessful)
{
	if (!bSuccessful)
	{
		SetStatus(TEXT("Failed to search for sessions."));
		return;
	}

	PopulateSessionList();
}

// ============================================================
// Session List Population
// ============================================================

void UZeroDawnMainMenuWidget::PopulateSessionList()
{
	if (!SessionResultsBox) return;

	SessionResultsBox->ClearChildren();

	UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get();
	if (!Subsystem)
	{
		SetStatus(TEXT("Session subsystem unavailable."));
		return;
	}

	const int32 NumResults = Subsystem->GetSearchResultCount();

	if (NumResults == 0)
	{
		SetStatus(TEXT("No sessions found. Try hosting one yourself!"));
		return;
	}

	SetStatus(FString::Printf(TEXT("Found %d session(s). Select one to join."), NumResults));

	// Create a button for each discovered session.
	for (int32 i = 0; i < NumResults; ++i)
	{
		FString ServerName, MapName, GameMode;
		int32 CurrentPlayers = 0, MaxPlayersCount = 0, Ping = 0;

		if (!Subsystem->GetSearchResultInfo(i, ServerName, MapName, GameMode, CurrentPlayers, MaxPlayersCount, Ping))
		{
			continue;
		}

		// Build a descriptive label for this session entry.
		const FString Label = FString::Printf(TEXT("%s | %s | %s | %d/%d | %dms"),
			*ServerName, *MapName, *GameMode, CurrentPlayers, MaxPlayersCount, Ping);

		// Create a button for this session.
		UButton* SessionButton = NewObject<UButton>(SessionResultsBox);
		if (SessionButton)
		{
			// Wrap the button in a text block that displays the session info.
			UTextBlock* ButtonLabel = NewObject<UTextBlock>(SessionButton);
			if (ButtonLabel)
			{
				ButtonLabel->SetText(FText::FromString(Label));
				ButtonLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
			}

			// Set the label as the button's single content widget.
			SessionButton->SetContent(ButtonLabel);

			// Bind the join action — capture index by value.
			const int32 CapturedIndex = i;
			SessionButton->OnClicked.AddLambda([this, CapturedIndex]()
			{
				OnJoinSessionClicked(CapturedIndex);
			});

			SessionResultsBox->AddChild(SessionButton);
		}
	}
}

// ============================================================
// Helpers
// ============================================================

void UZeroDawnMainMenuWidget::SetStatus(const FString& Message)
{
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(Message));
	}
}
