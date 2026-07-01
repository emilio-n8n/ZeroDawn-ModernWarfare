#include "ZeroDawnDominationGameMode.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "DominationZone.h"

AZeroDawnDominationGameMode::AZeroDawnDominationGameMode()
{
	GameModeType = EGameModeType::Domination;
	ScoreLimit = DominationScoreLimit;
}

void AZeroDawnDominationGameMode::BeginPlay()
{
	Super::BeginPlay();
	LastScoreTime = 0.0f;
}

void AZeroDawnDominationGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS || ZDGS->MatchState != EMatchState::InProgress) return;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastScoreTime >= ScoreInterval)
	{
		LastScoreTime = CurrentTime;

		int32 AlphaZones = 0, BravoZones = 0;
		TArray<AActor*> Zones;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADominationZone::StaticClass(), Zones);

		for (AActor* Zone : Zones)
		{
			ADominationZone* DZ = Cast<ADominationZone>(Zone);
			if (DZ)
			{
				if (DZ->ControllingTeam == ETeamType::Alpha) AlphaZones++;
				else if (DZ->ControllingTeam == ETeamType::Bravo) BravoZones++;
			}
		}

		if (AlphaZones > 0) ZDGS->AddTeamScore(ETeamType::Alpha, AlphaZones * PointsPerTick);
		if (BravoZones > 0) ZDGS->AddTeamScore(ETeamType::Bravo, BravoZones * PointsPerTick);
	}
}

void AZeroDawnDominationGameMode::CheckMatchEndConditions()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS) return;

	if (ZDGS->TeamAlphaScore >= DominationScoreLimit || ZDGS->TeamBravoScore >= DominationScoreLimit)
	{
		EndCurrentMatch();
	}
}

void AZeroDawnDominationGameMode::HandleMatchHasEnded()
{
	// Determine winner by comparing team scores before match ends
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		if (ZDGS->TeamAlphaScore > ZDGS->TeamBravoScore)
		{
			ZDGS->WinningTeam = ETeamType::Alpha;
		}
		else if (ZDGS->TeamBravoScore > ZDGS->TeamAlphaScore)
		{
			ZDGS->WinningTeam = ETeamType::Bravo;
		}
		else
		{
			ZDGS->WinningTeam = ETeamType::None;
		}
	}

	Super::HandleMatchHasEnded();
}
