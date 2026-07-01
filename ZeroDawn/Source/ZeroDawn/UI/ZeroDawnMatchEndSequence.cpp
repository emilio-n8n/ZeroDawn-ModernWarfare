#include "ZeroDawnMatchEndSequence.h"
#include "Blueprint/UserWidget.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "Kismet/GameplayStatics.h"

AZeroDawnMatchEndSequence::AZeroDawnMatchEndSequence()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
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
