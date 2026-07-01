#include "ZeroDawnMatchEndSequence.h"
#include "Blueprint/UserWidget.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AZeroDawnMatchEndSequence::AZeroDawnMatchEndSequence()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AZeroDawnMatchEndSequence::OnMatchEnd(ETeamType WinningTeam, int32 AlphaScore, int32 BravoScore, const TArray<FPlayerScoreData>& Scoreboard)
{
	if (!MatchEndWidgetClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	// Remove any existing match end widget
	if (MatchEndWidget && MatchEndWidget->IsInViewport())
	{
		MatchEndWidget->RemoveFromParent();
		MatchEndWidget = nullptr;
	}

	MatchEndWidget = CreateWidget<UUserWidget>(PC, MatchEndWidgetClass);
	if (!MatchEndWidget) return;

	// Notify the widget about match results by calling a Blueprint-implemented "Setup" function
	UFunction* SetupFunc = MatchEndWidget->FindFunction(FName("Setup"));
	if (SetupFunc)
	{
		struct FMatchEndParams
		{
			ETeamType WinningTeam;
			int32 AlphaScore;
			int32 BravoScore;
		};
		FMatchEndParams Params;
		Params.WinningTeam = WinningTeam;
		Params.AlphaScore = AlphaScore;
		Params.BravoScore = BravoScore;
		MatchEndWidget->ProcessEvent(SetupFunc, &Params);
	}

	MatchEndWidget->AddToViewport(100);
}

void AZeroDawnMatchEndSequence::ShowMatchEnd(ETeamType WinningTeam, int32 AlphaScore, int32 BravoScore, const TArray<FPlayerScoreData>& Scoreboard)
{
	OnMatchEnd(WinningTeam, AlphaScore, BravoScore, Scoreboard);

	TWeakObjectPtr<AZeroDawnMatchEndSequence> WeakThis(this);
	GetWorldTimerManager().SetTimer(ReturnToLobbyTimerHandle, [WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->ReturnToLobby();
		}
	}, MatchEndDelay, false);
}

void AZeroDawnMatchEndSequence::ShowTopPlayers(const TArray<FPlayerScoreData>& TopPlayers)
{
	OnShowTopPlayers(TopPlayers);
}

void AZeroDawnMatchEndSequence::OnShowTopPlayers(const TArray<FPlayerScoreData>& TopPlayers)
{
	if (!FinalScoreboardWidgetClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	// Remove any existing final scoreboard widget
	if (FinalScoreboardWidget && FinalScoreboardWidget->IsInViewport())
	{
		FinalScoreboardWidget->RemoveFromParent();
		FinalScoreboardWidget = nullptr;
	}

	FinalScoreboardWidget = CreateWidget<UUserWidget>(PC, FinalScoreboardWidgetClass);
	if (!FinalScoreboardWidget) return;

	// Notify the widget about top player data by calling a Blueprint-implemented "Setup" function
	UFunction* SetupFunc = FinalScoreboardWidget->FindFunction(FName("Setup"));
	if (SetupFunc)
	{
		struct FTopPlayersParams
		{
			TArray<FPlayerScoreData> TopPlayers;
		};
		FTopPlayersParams Params;
		Params.TopPlayers = TopPlayers;
		FinalScoreboardWidget->ProcessEvent(SetupFunc, &Params);
	}

	FinalScoreboardWidget->AddToViewport(100);
}

void AZeroDawnMatchEndSequence::ReturnToLobby()
{
	if (!GetWorld()) return;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());
		}
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyMap"));
}

void AZeroDawnMatchEndSequence::PlayMatchEndSound(ETeamType PlayerTeam, ETeamType WinningTeam)
{
	if (PlayerTeam == WinningTeam)
	{
		PlayVictoryMusic();
	}
	else
	{
		PlayDefeatMusic();
	}
}

void AZeroDawnMatchEndSequence::PlayVictoryMusic()
{
	if (VictoryMusicSound)
	{
		UGameplayStatics::PlaySound2D(this, VictoryMusicSound);
	}
}

void AZeroDawnMatchEndSequence::PlayDefeatMusic()
{
	if (DefeatMusicSound)
	{
		UGameplayStatics::PlaySound2D(this, DefeatMusicSound);
	}
}
