#include "ZeroDawnSaveGame.h"

UZeroDawnSaveGame::UZeroDawnSaveGame()
{
	PlayerLevel = 1;
	PlayerXP = 0;

	SavedLoadouts.SetNum(5);
	for (int32 i = 0; i < 5; i++)
	{
		SavedLoadouts[i].ClassName = FString::Printf(TEXT("Class %d"), i + 1);
		SavedLoadouts[i].PrimaryWeapon = EWeaponType::AssaultRifle;
		SavedLoadouts[i].SecondaryWeapon = EWeaponType::Pistol;
		SavedLoadouts[i].Perk1 = EPerkType::FlakJacket;
		SavedLoadouts[i].Perk2 = EPerkType::ColdBlooded;
		SavedLoadouts[i].Perk3 = EPerkType::Amped;
	}

	GraphicsQuality = 3;
	ResolutionScale = 100;
	MasterVolume = 1.0f;
	SFXVolume = 1.0f;
	MusicVolume = 0.7f;
	MouseSensitivity = 3.0f;
	ADSMultiplier = 0.5f;
	bInvertY = false;

	CareerKills = 0;
	CareerDeaths = 0;
	CareerWins = 0;
	CareerLosses = 0;
	CareerPlaytime = 0.0f;
}

bool UZeroDawnSaveGame::SaveProgress(UObject* WorldContext, const FString& PlayerId, UZeroDawnSaveGame* SaveData)
{
	if (!WorldContext || !SaveData) return false;

	const FString SlotName = GetSlotName(PlayerId);
	const int32 UserIndex = 0;

	return UGameplayStatics::SaveGameToSlot(SaveData, SlotName, UserIndex);
}

UZeroDawnSaveGame* UZeroDawnSaveGame::LoadProgress(const UObject* WorldContext, const FString& PlayerId)
{
	if (!WorldContext) return nullptr;

	const FString SlotName = GetSlotName(PlayerId);
	const int32 UserIndex = 0;

	USaveGame* LoadedData = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	if (!LoadedData) return nullptr;

	return Cast<UZeroDawnSaveGame>(LoadedData);
}

FString UZeroDawnSaveGame::GetSlotName(const FString& PlayerId)
{
	return FString::Printf(TEXT("ZeroDawnSave_%s"), *PlayerId);
}
