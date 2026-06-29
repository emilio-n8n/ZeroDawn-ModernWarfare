#include "ZeroDawnPlayerState.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../GameModes/ZeroDawnGameModeBase.h"

AZeroDawnPlayerState::AZeroDawnPlayerState()
{
	bReplicates = true;
	bAlwaysRelevant = true;
    SetNetUpdateFrequency(20.0f);

	PlayerLevel = 1;
	PlayerXP = 0;
	TotalKills = 0;
	TotalDeaths = 0;
	TotalAssists = 0;
	TotalScore = 0;
	CurrentKillstreak = 0;
	HighestKillstreak = 0;
}

void AZeroDawnPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AZeroDawnPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZeroDawnPlayerState, TotalKills);
	DOREPLIFETIME(AZeroDawnPlayerState, TotalDeaths);
	DOREPLIFETIME(AZeroDawnPlayerState, TotalAssists);
	DOREPLIFETIME(AZeroDawnPlayerState, TotalScore);
	DOREPLIFETIME(AZeroDawnPlayerState, CurrentKillstreak);
	DOREPLIFETIME(AZeroDawnPlayerState, HighestKillstreak);
	DOREPLIFETIME(AZeroDawnPlayerState, PlayerLevel);
	DOREPLIFETIME(AZeroDawnPlayerState, PlayerXP);
	DOREPLIFETIME(AZeroDawnPlayerState, PlayerTeam);
	DOREPLIFETIME(AZeroDawnPlayerState, bIsReady);
	DOREPLIFETIME(AZeroDawnPlayerState, bHasSpawned);
	DOREPLIFETIME(AZeroDawnPlayerState, CurrentLoadout);
}

void AZeroDawnPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PlayerState);
	if (ZDPS)
	{
		ZDPS->TotalKills = TotalKills;
		ZDPS->TotalDeaths = TotalDeaths;
		ZDPS->TotalAssists = TotalAssists;
		ZDPS->TotalScore = TotalScore;
		ZDPS->PlayerLevel = PlayerLevel;
		ZDPS->PlayerXP = PlayerXP;
	}
}

void AZeroDawnPlayerState::Reset()
{
	Super::Reset();
	TotalKills = 0;
	TotalDeaths = 0;
	TotalAssists = 0;
	TotalScore = 0;
	CurrentKillstreak = 0;
	HighestKillstreak = 0;
}

void AZeroDawnPlayerState::AddKill()
{
	if (!HasAuthority()) return;
	TotalKills++;
	CurrentKillstreak++;
	TotalScore += 100;

	if (CurrentKillstreak > HighestKillstreak)
	{
		HighestKillstreak = CurrentKillstreak;
	}
}

void AZeroDawnPlayerState::AddDeath()
{
	if (!HasAuthority()) return;
	TotalDeaths++;
	CurrentKillstreak = 0;
}

void AZeroDawnPlayerState::AddAssist()
{
	if (!HasAuthority()) return;
	TotalAssists++;
	TotalScore += 25;
}

void AZeroDawnPlayerState::AddScorePoints(int32 Amount)
{
	if (!HasAuthority()) return;
	TotalScore += Amount;
}

void AZeroDawnPlayerState::RequestRespawn()
{
	if (!HasAuthority()) return;
	ClientRequestRespawn();
}

void AZeroDawnPlayerState::ClientRequestRespawn_Implementation()
{
	AZeroDawnGameModeBase* GM = GetWorld()->GetAuthGameMode<AZeroDawnGameModeBase>();
	if (GM)
	{
		GM->RespawnPlayer(Cast<AController>(GetOwner()));
	}
}

FPlayerScoreData AZeroDawnPlayerState::GetScoreData() const
{
	FPlayerScoreData Data;
	Data.PlayerName = GetPlayerName();
	Data.Kills = TotalKills;
	Data.Deaths = TotalDeaths;
	Data.Assists = TotalAssists;
	Data.Score = TotalScore;
	Data.Ping = int32(GetPingInMilliseconds());
	Data.Team = PlayerTeam;
	Data.Killstreak = CurrentKillstreak;
	return Data;
}
