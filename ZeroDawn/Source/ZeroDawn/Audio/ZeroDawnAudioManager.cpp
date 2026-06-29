#include "ZeroDawnAudioManager.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UZeroDawnAudioManager::UZeroDawnAudioManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);

	MusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
	AmbientComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientComponent"));
}

void UZeroDawnAudioManager::PlayGunshot(EWeaponType WeaponType, FVector Location)
{
	if (WeaponFireSounds.Contains(WeaponType) && WeaponFireSounds[WeaponType])
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponFireSounds[WeaponType], Location, 1.0f);
	}
}

void UZeroDawnAudioManager::PlayFootstep(FVector Location, float Volume)
{
	if (FootstepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, Location, Volume);
	}
}

void UZeroDawnAudioManager::PlayImpactSound(FVector Location, EPhysicalSurface SurfaceType) {}

void UZeroDawnAudioManager::PlayDeathSound(FVector Location)
{
	if (DeathSoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSoundEffect, Location);
	}
}

void UZeroDawnAudioManager::PlayVoiceLine(const FString& LineID)
{
	if (VoiceLines.Contains(LineID) && VoiceLines[LineID])
	{
		UGameplayStatics::PlaySound2D(this, VoiceLines[LineID]);
	}
}

void UZeroDawnAudioManager::StartCombatMusic()
{
	if (MusicComponent && CombatMusicLoop && !MusicComponent->IsPlaying())
	{
		MusicComponent->SetSound(CombatMusicLoop);
		MusicComponent->Play();
	}
}

void UZeroDawnAudioManager::StopCombatMusic()
{
	if (MusicComponent)
	{
		MusicComponent->Stop();
	}
}

void UZeroDawnAudioManager::SetAmbientVolume(float Volume)
{
	if (AmbientComponent)
	{
		AmbientComponent->SetVolumeMultiplier(Volume);
	}
}

void UZeroDawnAudioManager::SetMasterVolume(float Volume)
{
	if (MusicComponent) MusicComponent->SetVolumeMultiplier(Volume);
	if (AmbientComponent) AmbientComponent->SetVolumeMultiplier(Volume);
}

void UZeroDawnAudioManager::PlayUISound(const FString& SoundName)
{
	if (UISounds.Contains(SoundName) && UISounds[SoundName])
	{
		UGameplayStatics::PlaySound2D(this, UISounds[SoundName]);
	}
}
