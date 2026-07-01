#include "ZeroDawnLobbyWidget.h"
#include "../Multiplayer/ZeroDawnSessionSubsystem.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"

// ============================================================
// Lifecycle
// ============================================================

void UZeroDawnLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Cache the session subsystem.
	if (UGameInstance* GI = GetGameInstance())
	{
		SessionSubsystem = GI->GetSubsystem<UZeroDawnSessionSubsystem>();
	}

	// Map game mode enum values to display strings.
	GameModeMap.Empty();
	GameModeMap.Add(TEXT("Team Deathmatch"), EGameModeType::TeamDeathmatch);
	GameModeMap.Add(TEXT("Free For All"),    EGameModeType::FreeForAll);
	GameModeMap.Add(TEXT("Domination"),      EGameModeType::Domination);
	GameModeMap.Add(TEXT("Search & Destroy"),EGameModeType::SearchAndDestroy);
	GameModeMap.Add(TEXT("Hardpoint"),       EGameModeType::Hardpoint);

	// Populate the game mode combo box and select the default.
	if (GameModeSelector)
	{
		GameModeSelector->ClearOptions();

		TArray<FString> DisplayNames;
		GameModeMap.GenerateKeyArray(DisplayNames);
		DisplayNames.Sort();

		for (const FString& Name : DisplayNames)
		{
			GameModeSelector->AddOption(Name);
		}

		// Select the first option (Team Deathmatch) by default.
		GameModeSelector->SetSelectedOption(TEXT("Team Deathmatch"));
		GameModeSelector->OnSelectionChanged.AddDynamic(this, &UZeroDawnLobbyWidget::OnGameModeChanged);
	}

	// Bind button events.
	if (ReadyToggleButton)
	{
		ReadyToggleButton->OnClicked.AddDynamic(this, &UZeroDawnLobbyWidget::OnReadyToggleClicked);
	}

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UZeroDawnLobbyWidget::OnStartGameClicked);
	}

	// Initialise the ready button text.
	if (ReadyButtonText)
	{
		ReadyButtonText->SetText(FText::FromString(TEXT("Not Ready")));
	}

	SetStatus(TEXT("Lobby: Press Ready when you're set, or select a game mode."));
}

void UZeroDawnLobbyWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UZeroDawnLobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Periodically refresh the player list to pick up new joiners / ready-state changes.
	RefreshTimer += InDeltaTime;
	if (RefreshTimer >= RefreshInterval)
	{
		RefreshTimer = 0.0f;
		RefreshPlayerList();
	}
}

// ============================================================
// Button Handlers
// ============================================================

void UZeroDawnLobbyWidget::OnReadyToggleClicked()
{
	bLocalReady = !bLocalReady;

	// Update the local button text.
	if (ReadyButtonText)
	{
		ReadyButtonText->SetText(FText::FromString(bLocalReady ? TEXT("Ready") : TEXT("Not Ready")));
	}

	// Send the ready state change to the server so it is replicated.
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AZeroDawnPlayerState* ZDPS = PC->GetPlayerState<AZeroDawnPlayerState>();
		if (ZDPS)
		{
			ZDPS->ServerSetReady(bLocalReady);
		}
	}
}

void UZeroDawnLobbyWidget::OnGameModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (GameModeMap.Contains(SelectedItem))
	{
		SelectedGameMode = GameModeMap[SelectedItem];
		SetStatus(FString::Printf(TEXT("Game mode changed to %s."), *SelectedItem));
	}
}

void UZeroDawnLobbyWidget::OnStartGameClicked()
{
	// Only the host can start the game.
	const UZeroDawnSessionSubsystem* Subsystem = SessionSubsystem.Get();
	if (Subsystem && !Subsystem->IsHost())
	{
		SetStatus(TEXT("Only the host can start the game."));
		return;
	}

	// Build the map name based on the selected game mode.
	// In a full implementation this would read from a level-configuration data asset.
	FString MapName;
	switch (SelectedGameMode)
	{
	case EGameModeType::TeamDeathmatch:
		MapName = TEXT("Nuketown");
		break;
	case EGameModeType::FreeForAll:
		MapName = TEXT("Nuketown");
		break;
	case EGameModeType::Domination:
		MapName = TEXT("DominationMap");
		break;
	case EGameModeType::SearchAndDestroy:
		MapName = TEXT("SDMap");
		break;
	case EGameModeType::Hardpoint:
		MapName = TEXT("HardpointMap");
		break;
	default:
		MapName = TEXT("Nuketown");
		break;
	}

	// Travel the server (and all clients via SeamlessTravel) to the game map.
	UWorld* World = GetWorld();
	if (World)
	{
		const FString URL = FString::Printf(TEXT("/Game/Maps/%s?listen"), *MapName);
		World->ServerTravel(URL);
		SetStatus(FString::Printf(TEXT("Starting %s on %s..."), *GameModeToString(SelectedGameMode), *MapName));
	}
}

// ============================================================
// Player List
// ============================================================

void UZeroDawnLobbyWidget::RefreshPlayerList()
{
	if (!PlayerListBox) return;

	PlayerListBox->ClearChildren();

	AGameStateBase* GS = GetWorld() ? GetWorld()->GetGameState() : nullptr;
	if (!GS) return;

	bool bHasPlayers = false;

	for (APlayerState* PS : GS->PlayerArray)
	{
		AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PS);
		if (!ZDPS) continue;

		bHasPlayers = true;

		// Build the player entry: "PlayerName [Ready] [Team]"
		const FString ReadyStr = ZDPS->bIsReady ? TEXT("Ready") : TEXT("Not Ready");
		const FString TeamStr = StaticEnum<ETeamType>()->GetDisplayValueAsText(ZDPS->PlayerTeam).ToString();
		const FString Entry = FString::Printf(TEXT("%s — %s — %s"),
			*ZDPS->GetPlayerName(), *ReadyStr, *TeamStr);

		UTextBlock* EntryText = NewObject<UTextBlock>(this);
		if (EntryText)
		{
			EntryText->SetText(FText::FromString(Entry));
			EntryText->SetColorAndOpacity(FSlateColor(ZDPS->bIsReady ? FLinearColor::Green : FLinearColor::White));
			PlayerListBox->AddChild(EntryText);
		}
	}

	if (!bHasPlayers)
	{
		UTextBlock* EmptyText = NewObject<UTextBlock>(this);
		if (EmptyText)
		{
			EmptyText->SetText(FText::FromString(TEXT("No players in lobby yet...")));
			EmptyText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
			PlayerListBox->AddChild(EmptyText);
		}
	}
}

// ============================================================
// Helpers
// ============================================================

void UZeroDawnLobbyWidget::SetStatus(const FString& Message)
{
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(Message));
	}
}

FString UZeroDawnLobbyWidget::GameModeToString(EGameModeType Mode) const
{
	switch (Mode)
	{
	case EGameModeType::TeamDeathmatch:   return TEXT("Team Deathmatch");
	case EGameModeType::FreeForAll:        return TEXT("Free For All");
	case EGameModeType::Domination:        return TEXT("Domination");
	case EGameModeType::SearchAndDestroy:  return TEXT("Search & Destroy");
	case EGameModeType::Hardpoint:         return TEXT("Hardpoint");
	default:                               return TEXT("Unknown");
	}
}
