#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnAudioManager.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnAudioManager();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayGunshot(EWeaponType WeaponType, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayFootstep(FVector Location, float Volume = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayImpactSound(FVector Location, EPhysicalSurface SurfaceType);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayDeathSound(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayVoiceLine(const FString& LineID);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StartCombatMusic();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopCombatMusic();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetAmbientVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetMasterVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayUISound(const FString& SoundName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TMap<EWeaponType, USoundCue*> WeaponFireSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* FootstepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* DeathSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* CombatMusicLoop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* AmbientSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TMap<FString, USoundCue*> UISounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TMap<FString, USoundCue*> VoiceLines;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* MusicComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* AmbientComponent;
};
