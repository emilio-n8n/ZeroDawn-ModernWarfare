#include "ZeroDawnPlayerState.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../GameModes/ZeroDawnGameModeBase.h"
#include "../Progression/ZeroDawnSaveGame.h"
#include "../Progression/ZeroDawnCreateAClass.h"
#include "../UI/ZeroDawnSettingsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

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
	CareerWins = 0;
	CareerLosses = 0;
}

void AZeroDawnPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Load saved progress on the client where save files exist
	if (!HasAuthority())
	{
		LoadSavedProgress();
	}
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
	DOREPLIFETIME(AZeroDawnPlayerState, CareerWins);
	DOREPLIFETIME(AZeroDawnPlayerState, CareerLosses);
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
		ZDPS->CareerWins = CareerWins;
		ZDPS->CareerLosses = CareerLosses;
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
	// Note: PlayerLevel, PlayerXP, CareerWins, CareerLosses are NOT reset here -
	// they persist across matches within a session.
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

void AZeroDawnPlayerState::AddWin()
{
	if (!HasAuthority()) return;
	CareerWins++;
}

void AZeroDawnPlayerState::AddLoss()
{
	if (!HasAuthority()) return;
	CareerLosses++;
}

void AZeroDawnPlayerState::RecordWeaponKill(EWeaponType WeaponType)
{
	if (!HasAuthority()) return;

	if (WeaponUsageMap.Contains(WeaponType))
	{
		WeaponUsageMap[WeaponType] += 1;
	}
	else
	{
		WeaponUsageMap.Add(WeaponType, 1);
	}
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

// ============================================================
// Save / Load System Implementation
// ============================================================

void AZeroDawnPlayerState::ClientSaveGameData_Implementation()
{
	SaveCurrentProgress();
}

void AZeroDawnPlayerState::SaveCurrentProgress()
{
	const FString PlayerId = GetSavePlayerId();

	// Try to load existing save data first so we don't overwrite career stats
	UZeroDawnSaveGame* SaveData = UZeroDawnSaveGame::LoadProgress(this, PlayerId);
	if (!SaveData)
	{
		// No existing save — create a new one
		SaveData = NewObject<UZeroDawnSaveGame>();
	}

	// --- Progression ---
	SaveData->PlayerLevel = PlayerLevel;
	SaveData->PlayerXP = PlayerXP;

	// --- Loadouts from CreateAClass ---
	if (AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(GetPawn()))
	{
		if (UZeroDawnCreateAClass* CAC = Char->FindComponentByClass<UZeroDawnCreateAClass>())
		{
			SaveData->SavedLoadouts = CAC->SavedLoadouts;
		}
	}

	// --- Settings from SettingsMenu actors in the world ---
	// Find the first settings menu actor to grab current settings values
	AZeroDawnSettingsMenu* SettingsMenu = nullptr;
	if (UWorld* World = GetWorld())
	{
		// Look for settings menu actor (spawned on the client for this player)
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC && PC->IsLocalController())
		{
			// The settings actor may be owned by the player or exist in the world
			// We use TActorIterator to find the first one owned by this controller
			for (TActorIterator<AZeroDawnSettingsMenu> It(World); It; ++It)
			{
				AZeroDawnSettingsMenu* Menu = *It;
				if (Menu && Menu->GetOwner() == PC)
				{
					SettingsMenu = Menu;
					break;
				}
			}
		}

		// Fallback: if no owned settings menu found, use any settings menu in the world
		if (!SettingsMenu)
		{
			for (TActorIterator<AZeroDawnSettingsMenu> It(World); It; ++It)
			{
				SettingsMenu = *It;
				break;
			}
		}
	}

	if (SettingsMenu)
	{
		SaveData->GraphicsQuality = SettingsMenu->GraphicsQuality;
		SaveData->ResolutionScale = 100; // Default resolution scale
		SaveData->MasterVolume = SettingsMenu->MasterVolume;
		SaveData->SFXVolume = SettingsMenu->SFXVolume;
		SaveData->MusicVolume = SettingsMenu->MusicVolume;
		SaveData->MouseSensitivity = SettingsMenu->MouseSensitivity;
		SaveData->ADSMultiplier = SettingsMenu->ADSMultiplier;
		SaveData->bInvertY = SettingsMenu->bInvertY;
	}

	// --- Career Stats (accumulate) ---
	// Add current match stats to career totals
	SaveData->CareerKills += TotalKills;
	SaveData->CareerDeaths += TotalDeaths;
	SaveData->CareerWins = CareerWins;
	SaveData->CareerLosses = CareerLosses;

	// Playtime: approximate by tracking session time
	if (UWorld* World = GetWorld())
	{
		SaveData->CareerPlaytime += World->GetTimeSeconds() / 60.0f; // Convert to minutes
	}

	// --- Weapon Usage ---
	SaveData->WeaponUsageMap = WeaponUsageMap;

	// Persist to disk
	UZeroDawnSaveGame::SaveProgress(this, PlayerId, SaveData);
}

void AZeroDawnPlayerState::LoadSavedProgress()
{
	const FString PlayerId = GetSavePlayerId();

	UZeroDawnSaveGame* SaveData = UZeroDawnSaveGame::LoadProgress(this, PlayerId);
	if (!SaveData) return;

	// Restore progression
	PlayerLevel = SaveData->PlayerLevel;
	PlayerXP = SaveData->PlayerXP;
	CareerWins = SaveData->CareerWins;
	CareerLosses = SaveData->CareerLosses;

	// Restore loadouts onto the character's CreateAClass component
	if (AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(GetPawn()))
	{
		if (UZeroDawnCreateAClass* CAC = Char->FindComponentByClass<UZeroDawnCreateAClass>())
		{
			CAC->SavedLoadouts = SaveData->SavedLoadouts;

			// If the save has at least one valid loadout, use it as current
			if (SaveData->SavedLoadouts.Num() > 0)
			{
				CAC->CurrentLoadout = SaveData->SavedLoadouts[0];
			}
		}
	}

	// Restore settings: find the settings menu and apply saved values
	// This is handled separately, as the settings menu reads its own defaults.
	// Connect settings menu by updating its UPROPERTY values that get applied.
	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		// If we have a player controller that is locally controlled
		if (PC && PC->IsLocalController())
		{
			// Trigger a settings apply with the saved values
			// by finding an available settings menu actor
			AZeroDawnSettingsMenu* SettingsMenu = nullptr;
			for (TActorIterator<AZeroDawnSettingsMenu> It(World); It; ++It)
			{
				AZeroDawnSettingsMenu* Menu = *It;
				if (Menu && Menu->GetOwner() == PC)
				{
					SettingsMenu = Menu;
					break;
				}
			}

			if (SettingsMenu)
			{
				// Apply saved settings
				SettingsMenu->ApplyGraphicsSettings(SaveData->GraphicsQuality, SaveData->ResolutionScale);
				SettingsMenu->ApplyAudioSettings(SaveData->MasterVolume, SaveData->SFXVolume, SaveData->MusicVolume);
				SettingsMenu->ApplyControlsSettings(SaveData->MouseSensitivity, SaveData->ADSMultiplier, SaveData->bInvertY);
			}
		}
	}
}

FString AZeroDawnPlayerState::GetSavePlayerId() const
{
	// Use the online unique ID if available (persistent across sessions)
	if (GetUniqueId().IsValid())
	{
		return GetUniqueId()->ToString();
	}

	// Fall back to player name (might not be unique but workable)
	FString PlayerName = GetPlayerName();
	if (!PlayerName.IsEmpty())
	{
		return PlayerName;
	}

	// Last resort: default slot
	return TEXT("Default");
}
