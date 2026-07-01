#include "ZeroDawnGameState.h"
#include "ZeroDawnPlayerState.h"
#include "../Character/ZeroDawnCharacter.h"

AZeroDawnGameState::AZeroDawnGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AZeroDawnGameState::BeginPlay()
{
	Super::BeginPlay();
	RemainingPreMatchTime = PreMatchCountdown;
}

void AZeroDawnGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZeroDawnGameState, MatchState);
	DOREPLIFETIME(AZeroDawnGameState, CurrentGameMode);
	DOREPLIFETIME(AZeroDawnGameState, MatchTime);
	DOREPLIFETIME(AZeroDawnGameState, MaxMatchTime);
	DOREPLIFETIME(AZeroDawnGameState, ScoreLimit);
	DOREPLIFETIME(AZeroDawnGameState, TeamAlphaScore);
	DOREPLIFETIME(AZeroDawnGameState, TeamBravoScore);
	DOREPLIFETIME(AZeroDawnGameState, PlayerScoreboard);
	DOREPLIFETIME(AZeroDawnGameState, RemainingPreMatchTime);
	DOREPLIFETIME(AZeroDawnGameState, WinningTeam);
}

void AZeroDawnGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AZeroDawnGameState::AddTeamScore(ETeamType Team, int32 Amount)
{
	if (!HasAuthority()) return;

	if (Team == ETeamType::Alpha)
		TeamAlphaScore += Amount;
	else if (Team == ETeamType::Bravo)
		TeamBravoScore += Amount;
}

void AZeroDawnGameState::SetMatchState(EMatchState NewState)
{
	if (!HasAuthority()) return;
	MatchState = NewState;
}

void AZeroDawnGameState::UpdateScoreboard()
{
	if (!HasAuthority()) return;

	PlayerScoreboard.Empty();

	for (APlayerState* PS : PlayerArray)
	{
		AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PS);
		if (ZDPS)
		{
			PlayerScoreboard.Add(ZDPS->GetScoreData());
		}
	}

	PlayerScoreboard.Sort([](const FPlayerScoreData& A, const FPlayerScoreData& B)
	{
		return A.Score > B.Score;
	});
}

FString AZeroDawnGameState::GetMatchTimeFormatted() const
{
	int32 TotalSeconds = FMath::FloorToInt(MaxMatchTime - MatchTime);
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;
	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

int32 AZeroDawnGameState::GetPlayerCount() const
{
	return PlayerArray.Num();
}
