#include "ZeroDawnGameModeBase.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "../Multiplayer/ZeroDawnPlayerController.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../WeaponSystem/ZeroDawnWeaponComponent.h"
#include "../Weapon/ZeroDawnWeapon.h"
#include "../UI/ZeroDawnHUD.h"
#include "Engine/PlayerStartPIE.h"

AZeroDawnGameModeBase::AZeroDawnGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	GameStateClass = AZeroDawnGameState::StaticClass();
	PlayerStateClass = AZeroDawnPlayerState::StaticClass();
	PlayerControllerClass = AZeroDawnPlayerController::StaticClass();

	bUseSeamlessTravel = true;
	bDelayedStart = true;
	bStartPlayersAsSpectators = false;
}

void AZeroDawnGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FindSpawnPoints();
	HandleMatchIsWaitingToStart();
}

void AZeroDawnGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS) return;

	if (ZDGS->MatchState == EMatchState::InProgress)
	{
		ZDGS->MatchTime += DeltaSeconds;

		if (ZDGS->MatchTime >= MatchTimeLimit)
		{
			EndCurrentMatch();
		}

		CheckMatchEndConditions();
	}
	else if (ZDGS->MatchState == EMatchState::PreMatch)
	{
		ZDGS->RemainingPreMatchTime -= DeltaSeconds;
		if (ZDGS->RemainingPreMatchTime <= 0.0f)
		{
			StartMatch();
		}
	}
}

void AZeroDawnGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AZeroDawnPlayerState* ZDPS = NewPlayer->GetPlayerState<AZeroDawnPlayerState>();
	if (ZDPS)
	{
		int32 AlphaCount = 0, BravoCount = 0;
		for (APlayerState* PS : GameState->PlayerArray)
		{
			AZeroDawnPlayerState* ZPS = Cast<AZeroDawnPlayerState>(PS);
			if (ZPS && ZPS != ZDPS)
			{
				if (ZPS->PlayerTeam == ETeamType::Alpha) AlphaCount++;
				else if (ZPS->PlayerTeam == ETeamType::Bravo) BravoCount++;
			}
		}
		ZDPS->PlayerTeam = (AlphaCount <= BravoCount) ? ETeamType::Alpha : ETeamType::Bravo;
	}

	RestartPlayer(NewPlayer);
}

void AZeroDawnGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AZeroDawnGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if (!NewPlayer) return;

	// If the player controller is in spectator mode, exit it before respawning
	AZeroDawnPlayerController* ZDPC = Cast<AZeroDawnPlayerController>(NewPlayer);
	if (ZDPC && ZDPC->bIsSpectating)
	{
		ZDPC->ExitSpectatorMode();
	}

	AZeroDawnCharacter* ExistingPawn = Cast<AZeroDawnCharacter>(NewPlayer->GetPawn());
	if (ExistingPawn)
	{
		ExistingPawn->Destroy();
	}

	RestartPlayerAtPlayerStart(NewPlayer, ChoosePlayerStart(NewPlayer));
}

void AZeroDawnGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	if (!NewPlayer || !StartSpot) return;

	AZeroDawnCharacter* NewPawn = SpawnDefaultPawnForController(NewPlayer);
	if (NewPawn)
	{
		NewPawn->SetActorLocation(StartSpot->GetActorLocation());
		NewPawn->SetActorRotation(StartSpot->GetActorRotation());

		NewPlayer->Possess(NewPawn);

		AZeroDawnPlayerState* ZDPS = NewPlayer->GetPlayerState<AZeroDawnPlayerState>();
		if (ZDPS)
		{
			ZDPS->bHasSpawned = true;
			ZDPS->bIsReady = true;
		}
	}
}

AActor* AZeroDawnGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	AZeroDawnPlayerState* ZDPS = Player->GetPlayerState<AZeroDawnPlayerState>();
	TArray<AActor*> PreferredStarts;

	if (ZDPS)
	{
		if (ZDPS->PlayerTeam == ETeamType::Alpha)
			PreferredStarts = AlphaSpawnPoints;
		else if (ZDPS->PlayerTeam == ETeamType::Bravo)
			PreferredStarts = BravoSpawnPoints;
	}

	if (PreferredStarts.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PreferredStarts.Num() - 1);
		return PreferredStarts[RandomIndex];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

bool AZeroDawnGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

AZeroDawnCharacter* AZeroDawnGameModeBase::SpawnDefaultPawnForController(AController* NewPlayer)
{
	if (!DefaultPawnClass) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.Instigator = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform SpawnTransform;
	return GetWorld()->SpawnActorDeferred<AZeroDawnCharacter>(DefaultPawnClass, SpawnTransform, NewPlayer, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
}

void AZeroDawnGameModeBase::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		ZDGS->SetMatchState(EMatchState::WaitingForPlayers);
		ZDGS->MatchTime = 0.0f;
		ZDGS->TeamAlphaScore = 0;
		ZDGS->TeamBravoScore = 0;
	}
}

void AZeroDawnGameModeBase::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	StartPreMatchCountdown();
}

void AZeroDawnGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		ZDGS->SetMatchState(EMatchState::MatchEnded);
		ZDGS->UpdateScoreboard();
	}

	OnMatchEnd();
}

void AZeroDawnGameModeBase::StartPreMatchCountdown()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		ZDGS->SetMatchState(EMatchState::PreMatch);
		ZDGS->RemainingPreMatchTime = PreMatchTime;
	}
}

void AZeroDawnGameModeBase::StartMatch()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		ZDGS->SetMatchState(EMatchState::InProgress);
		ZDGS->MatchTime = 0.0f;
	}
}

void AZeroDawnGameModeBase::EndCurrentMatch()
{
	if (GetMatchState() == MatchState::WaitingPostMatch) return;
	SetMatchState(MatchState::WaitingPostMatch);
	HandleMatchHasEnded();
}

bool AZeroDawnGameModeBase::IsMatchActive() const
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	return ZDGS && ZDGS->MatchState == EMatchState::InProgress;
}

void AZeroDawnGameModeBase::ScoreKill(AController* Killer, AController* Victim, AActor* DamageCauser)
{
	if (!Killer || !Victim) return;

	AZeroDawnPlayerState* KillerPS = Killer->GetPlayerState<AZeroDawnPlayerState>();
	AZeroDawnPlayerState* VictimPS = Victim->GetPlayerState<AZeroDawnPlayerState>();

	if (!KillerPS || !VictimPS) return;

	if (KillerPS->PlayerTeam == VictimPS->PlayerTeam)
	{
		ScoreTeamKill(Killer);
		return;
	}

	KillerPS->AddKill();
	VictimPS->AddDeath();

	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (ZDGS)
	{
		ZDGS->AddTeamScore(KillerPS->PlayerTeam, 1);
		ZDGS->UpdateScoreboard();
	}
}

void AZeroDawnGameModeBase::ScoreTeamKill(AController* Killer)
{
	AZeroDawnPlayerState* KillerPS = Killer->GetPlayerState<AZeroDawnPlayerState>();
	if (KillerPS)
	{
		KillerPS->AddDeath();
		KillerPS->AddScorePoints(-50);
	}
}

void AZeroDawnGameModeBase::RespawnPlayer(AController* Player)
{
	if (!Player || !HasAuthority()) return;

	RestartPlayer(Player);
}

void AZeroDawnGameModeBase::CheckMatchEndConditions()
{
	AZeroDawnGameState* ZDGS = GetZDGameState();
	if (!ZDGS || GameModeType != EGameModeType::FreeForAll) return;

	if (ZDGS->MatchTime >= MatchTimeLimit)
	{
		EndCurrentMatch();
	}
}

void AZeroDawnGameModeBase::OnPlayerKilled(AController* Killer, AController* Victim) {}

void AZeroDawnGameModeBase::OnMatchEnd()
{
	// Save progress for all players on match end
	for (APlayerState* PS : GameState->PlayerArray)
	{
		AZeroDawnPlayerState* ZDPS = Cast<AZeroDawnPlayerState>(PS);
		if (ZDPS)
		{
			// Determine win/loss for this player (if team-based)
			AZeroDawnGameState* ZDGS = GetZDGameState();
			if (ZDGS && ZDGS->MatchState == EMatchState::MatchEnded)
			{
				bool bPlayerWon = false;

				if (ZDPS->PlayerTeam == ETeamType::Alpha && ZDGS->TeamAlphaScore > ZDGS->TeamBravoScore)
				{
					bPlayerWon = true;
				}
				else if (ZDPS->PlayerTeam == ETeamType::Bravo && ZDGS->TeamBravoScore > ZDGS->TeamAlphaScore)
				{
					bPlayerWon = true;
				}

				if (bPlayerWon)
				{
					ZDPS->AddWin();
				}
				else
				{
					ZDPS->AddLoss();
				}
			}

			// Trigger client-side save
			ZDPS->ClientSaveGameData();
		}
	}
}

void AZeroDawnGameModeBase::FindSpawnPoints()
{
	AlphaSpawnPoints.Empty();
	BravoSpawnPoints.Empty();
	NeutralSpawnPoints.Empty();

	TArray<AActor*> FoundStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundStarts);

	for (AActor* Start : FoundStarts)
	{
		APlayerStart* PS = Cast<APlayerStart>(Start);
		if (PS)
		{
			if (PS->PlayerStartTag == FName("Alpha"))
				AlphaSpawnPoints.Add(PS);
			else if (PS->PlayerStartTag == FName("Bravo"))
				BravoSpawnPoints.Add(PS);
			else
				NeutralSpawnPoints.Add(PS);
		}
	}
}

AZeroDawnGameState* AZeroDawnGameModeBase::GetZDGameState() const
{
	return Cast<AZeroDawnGameState>(GameState);
}
