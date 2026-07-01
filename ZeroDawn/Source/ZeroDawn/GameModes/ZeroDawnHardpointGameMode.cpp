#include "ZeroDawnHardpointGameMode.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "HardpointZone.h"

AZeroDawnHardpointGameMode::AZeroDawnHardpointGameMode()
{
	GameModeType = EGameModeType::Hardpoint;
	ScoreLimit = HardpointScoreLimit;
}

void AZeroDawnHardpointGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHardpointZone::StaticClass(), FoundZones);

	for (AActor* Zone : FoundZones)
	{
		AHardpointZone* HP = Cast<AHardpointZone>(Zone);
		if (HP)
		{
			AllHardpoints.Add(HP);
			HP->SetActive(false);
		}
	}

	if (AllHardpoints.Num() > 0)
	{
		ActivateNextHardpoint();
	}
}

void AZeroDawnHardpointGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS || ZDGS->MatchState != EMatchState::InProgress) return;

	if (CurrentHardpointIndex < AllHardpoints.Num())
	{
		HardpointActiveTime += DeltaSeconds;

		if (HardpointActiveTime >= HardpointRotationTime)
		{
			AllHardpoints[CurrentHardpointIndex]->SetActive(false);
			CurrentHardpointIndex++;
			if (CurrentHardpointIndex < AllHardpoints.Num())
			{
				ActivateNextHardpoint();
			}
			else
			{
				// All hardpoints exhausted — loop back to index 0
				CurrentHardpointIndex = 0;
				ActivateNextHardpoint();
			}
			HardpointActiveTime = 0.0f;
			return;
		}

		AHardpointZone* ActiveHP = AllHardpoints[CurrentHardpointIndex];
		if (ActiveHP && ActiveHP->bIsActive && ActiveHP->ControllingTeam != ETeamType::None)
		{
			float CurrentTime = GetWorld()->GetTimeSeconds();
			if (CurrentTime - LastScoreTime >= 1.0f)
			{
				LastScoreTime = CurrentTime;
				ZDGS->AddTeamScore(ActiveHP->ControllingTeam, PointsPerSecond);
			}
		}
	}
}

void AZeroDawnHardpointGameMode::CheckMatchEndConditions()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS) return;

	if (ZDGS->TeamAlphaScore >= HardpointScoreLimit || ZDGS->TeamBravoScore >= HardpointScoreLimit)
	{
		EndCurrentMatch();
	}
}

void AZeroDawnHardpointGameMode::HandleMatchHasEnded()
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

void AZeroDawnHardpointGameMode::ActivateNextHardpoint()
{
	if (CurrentHardpointIndex < AllHardpoints.Num())
	{
		AllHardpoints[CurrentHardpointIndex]->SetActive(true);
		HardpointActiveTime = 0.0f;
	}
}
