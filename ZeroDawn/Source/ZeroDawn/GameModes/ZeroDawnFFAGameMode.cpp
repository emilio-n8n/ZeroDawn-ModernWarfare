#include "ZeroDawnFFAGameMode.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"

AZeroDawnFFAGameMode::AZeroDawnFFAGameMode()
{
	GameModeType = EGameModeType::FreeForAll;
	ScoreLimit = FFAScoreLimit;
}

void AZeroDawnFFAGameMode::PostLogin(APlayerController* NewPlayer)
{
	AZeroDawnPlayerState* ZDPS = NewPlayer->GetPlayerState<AZeroDawnPlayerState>();
	if (ZDPS)
	{
		ZDPS->PlayerTeam = ETeamType::None;
	}

	Super::Super::PostLogin(NewPlayer);
}

void AZeroDawnFFAGameMode::CheckMatchEndConditions()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS) return;

	for (const FPlayerScoreData& Data : ZDGS->PlayerScoreboard)
	{
		if (Data.Kills >= FFAScoreLimit)
		{
			EndCurrentMatch();
			return;
		}
	}
}
