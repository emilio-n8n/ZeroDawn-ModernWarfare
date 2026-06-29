#include "ZeroDawnTDMGameMode.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"

AZeroDawnTDMGameMode::AZeroDawnTDMGameMode()
{
	GameModeType = EGameModeType::TeamDeathmatch;
	ScoreLimit = TDMScoreLimit;
}

void AZeroDawnTDMGameMode::CheckMatchEndConditions()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS) return;

	if (ZDGS->TeamAlphaScore >= TDMScoreLimit || ZDGS->TeamBravoScore >= TDMScoreLimit)
	{
		EndCurrentMatch();
	}
	else if (ZDGS->MatchTime >= MatchTimeLimit)
	{
		EndCurrentMatch();
	}
}

void AZeroDawnTDMGameMode::OnPlayerKilled(AController* Killer, AController* Victim) {}
