#include "ZeroDawnSearchDestroyGameMode.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Interactive/ZeroDawnBomb.h"

AZeroDawnSearchDestroyGameMode::AZeroDawnSearchDestroyGameMode()
{
	GameModeType = EGameModeType::SearchAndDestroy;
	bUseSeamlessTravel = true;
	ScoreLimit = RoundsToWin;
	// MatchTimeLimit stays at 600.0f from base class (overall match timeout).
	// RoundTimeLimit (120.0f) is used per-round instead.
}

void AZeroDawnSearchDestroyGameMode::BeginPlay()
{
	AZeroDawnGameModeBase::BeginPlay();

	// Find and bind the bomb actor's delegates
	FindAndBindBomb();
}

void AZeroDawnSearchDestroyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bRoundActive) return;

	float RoundTime = GetWorld()->GetTimeSeconds() - RoundStartTime;
	if (RoundTime >= RoundTimeLimit && !bBombPlanted)
	{
		CountAlivePlayers();
		if (AlphaAlive > BravoAlive)
			EndRound(ETeamType::Alpha);
		else if (BravoAlive > AlphaAlive)
			EndRound(ETeamType::Bravo);
		else
			EndRound(ETeamType::None);
	}

	// Note: bomb timer is handled by the bomb actor's Tick() and its OnExploded delegate
	// The game mode receives the result via OnBombExploded() bound to the bomb's delegate.
}

void AZeroDawnSearchDestroyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AZeroDawnSearchDestroyGameMode::OnPlayerKilled(AController* Killer, AController* Victim)
{
	CountAlivePlayers();

	if (AlphaAlive <= 0)
	{
		EndRound(ETeamType::Bravo);
	}
	else if (BravoAlive <= 0)
	{
		EndRound(ETeamType::Alpha);
	}
}

void AZeroDawnSearchDestroyGameMode::CheckMatchEndConditions()
{
	if (AlphaRoundWins >= RoundsToWin || BravoRoundWins >= RoundsToWin)
	{
		EndCurrentMatch();
	}
}

void AZeroDawnSearchDestroyGameMode::StartNewRound()
{
	bRoundActive = true;
	bBombPlanted = false;
	BombPlantTime = 0.0f;
	RoundStartTime = GetWorld()->GetTimeSeconds();
	CurrentRound++;

	// Reset the bomb for the new round
	if (ActiveBomb)
	{
		ActiveBomb->ResetBomb();
	}

	if (!bSwitchedSides && CurrentRound > RoundsToWin)
	{
		bSwitchedSides = true;

		// Swap teams: Alpha <-> Bravo for all players
		for (APlayerState* PS : GameState->PlayerArray)
		{
			AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PS);
			if (ZDPS)
			{
				if (ZDPS->PlayerTeam == ETeamType::Alpha)
					ZDPS->PlayerTeam = ETeamType::Bravo;
				else if (ZDPS->PlayerTeam == ETeamType::Bravo)
					ZDPS->PlayerTeam = ETeamType::Alpha;
			}
		}
	}

	// Respawn all players
	for (APlayerState* PS : GameState->PlayerArray)
	{
		AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PS);
		if (ZDPS)
		{
			AController* PC = Cast<AController>(ZDPS->GetOwner());
			if (PC)
			{
				RespawnPlayer(PC);
			}
		}
	}
}

void AZeroDawnSearchDestroyGameMode::EndRound(ETeamType WinningTeam)
{
	if (!bRoundActive) return;
	bRoundActive = false;

	if (WinningTeam == ETeamType::Alpha)
		AlphaRoundWins++;
	else if (WinningTeam == ETeamType::Bravo)
		BravoRoundWins++;

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		ZDGS->TeamAlphaScore = AlphaRoundWins;
		ZDGS->TeamBravoScore = BravoRoundWins;
	}

	CheckMatchEndConditions();

	FTimerHandle NewRoundHandle;
	GetWorldTimerManager().SetTimer(NewRoundHandle, this, &AZeroDawnSearchDestroyGameMode::StartNewRound, 5.0f, false);
}

void AZeroDawnSearchDestroyGameMode::OnBombPlanted()
{
	bBombPlanted = true;
	BombPlantTime = GetWorld()->GetTimeSeconds();
}

void AZeroDawnSearchDestroyGameMode::OnBombDefused()
{
	EndRound(ETeamType::Alpha);
}

void AZeroDawnSearchDestroyGameMode::OnBombExploded()
{
	EndRound(ETeamType::Bravo);
}

void AZeroDawnSearchDestroyGameMode::CountAlivePlayers()
{
	AlphaAlive = 0;
	BravoAlive = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PS);
		if (!ZDPS) continue;

		AController* PC = Cast<AController>(ZDPS->GetOwner());
		if (!PC) continue;

		AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(PC->GetPawn());
		if (Char && Char->IsAlive())
		{
			if (ZDPS->PlayerTeam == ETeamType::Alpha)
				AlphaAlive++;
			else if (ZDPS->PlayerTeam == ETeamType::Bravo)
				BravoAlive++;
		}
	}
}

void AZeroDawnSearchDestroyGameMode::FindAndBindBomb()
{
	for (TActorIterator<AZeroDawnBomb> It(GetWorld()); It; ++It)
	{
		ActiveBomb = *It;
		break; // Only one bomb in S&D
	}

	if (!ActiveBomb)
	{
		UE_LOG(LogTemp, Warning, TEXT("S&D: No AZeroDawnBomb found in the level!"));
		return;
	}

	// Bind bomb delegates to game mode handlers
	ActiveBomb->OnBombPlantedDelegate.AddDynamic(this, &AZeroDawnSearchDestroyGameMode::OnBombPlanted);
	ActiveBomb->OnBombDefusedDelegate.AddDynamic(this, &AZeroDawnSearchDestroyGameMode::OnBombDefused);
	ActiveBomb->OnBombExplodedDelegate.AddDynamic(this, &AZeroDawnSearchDestroyGameMode::OnBombExploded);

	UE_LOG(LogTemp, Log, TEXT("S&D: Bomb actor bound to game mode delegates."));
}
