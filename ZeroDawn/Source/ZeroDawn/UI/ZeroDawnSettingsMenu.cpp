#include "ZeroDawnSettingsMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/ZeroDawnCharacter.h"

AZeroDawnSettingsMenu::AZeroDawnSettingsMenu()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	static ConstructorHelpers::FObjectFinder<USoundMix> SoundMixFinder(TEXT("/Game/ZeroDawn/Audio/ZeroDawnSoundMix.ZeroDawnSoundMix"));
	if (SoundMixFinder.Succeeded())
	{
		SoundMix = SoundMixFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SoundClassFinder(TEXT("/Game/ZeroDawn/Audio/ZeroDawnSoundClass.ZeroDawnSoundClass"));
	if (SoundClassFinder.Succeeded())
	{
		SoundClass = SoundClassFinder.Object;
	}
}

APlayerController* AZeroDawnSettingsMenu::GetPC() const
{
	return GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
}

void AZeroDawnSettingsMenu::OpenMainMenu()
{
	APlayerController* PC = GetPC();
	if (!PC) return;

	CloseMenu();

	if (MainMenuWidgetClass)
	{
		CurrentMenuWidget = CreateWidget<UUserWidget>(PC, MainMenuWidgetClass);
		if (CurrentMenuWidget)
		{
			CurrentMenuWidget->AddToViewport(100);
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void AZeroDawnSettingsMenu::OpenPauseMenu()
{
	APlayerController* PC = GetPC();
	if (!PC) return;

	CloseMenu();

	if (PauseMenuWidgetClass)
	{
		CurrentMenuWidget = CreateWidget<UUserWidget>(PC, PauseMenuWidgetClass);
		if (CurrentMenuWidget)
		{
			CurrentMenuWidget->AddToViewport(100);
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void AZeroDawnSettingsMenu::CloseMenu()
{
	if (CurrentMenuWidget)
	{
		CurrentMenuWidget->RemoveFromParent();
		CurrentMenuWidget = nullptr;
	}

	APlayerController* PC = GetPC();
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void AZeroDawnSettingsMenu::ApplyGraphicsSettings(int32 QualityLevel, int32 ResolutionScale)
{
	GraphicsQuality = FMath::Clamp(QualityLevel, 0, 5);

	auto SetCVar = [](const TCHAR* CVar, int32 Value)
	{
		IConsoleManager::Get().FindConsoleVariable(CVar)->Set(Value);
	};

	SetCVar(TEXT("r.ShadowQuality"), GraphicsQuality);
	SetCVar(TEXT("r.TextureQuality"), GraphicsQuality);
	SetCVar(TEXT("r.PostProcessQuality"), GraphicsQuality);
	SetCVar(TEXT("r.ViewDistanceScale"), GraphicsQuality * 0.25f + 0.5f);

	if (ResolutionScale > 0)
	{
		SetCVar(TEXT("r.ScreenPercentage"), FMath::Clamp(ResolutionScale, 50, 200));
	}
}

void AZeroDawnSettingsMenu::ApplyAudioSettings(float Master, float SFX, float Music)
{
	MasterVolume = FMath::Clamp(Master, 0.0f, 1.0f);
	SFXVolume = FMath::Clamp(SFX, 0.0f, 1.0f);
	MusicVolume = FMath::Clamp(Music, 0.0f, 1.0f);

	if (SoundMix && SoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, MasterVolume, 1.0f, 0.0f);
	}
}

void AZeroDawnSettingsMenu::ApplyControlsSettings(float Sensitivity, float ADSMult, bool InvertY)
{
	MouseSensitivity = FMath::Clamp(Sensitivity, 0.1f, 20.0f);
	ADSMultiplier = FMath::Clamp(ADSMult, 0.1f, 1.0f);
	bInvertY = InvertY;

	APlayerController* PC = GetPC();
	if (PC)
	{
		if (PC->PlayerInput)
		{
			PC->PlayerInput->SetMouseSensitivity(MouseSensitivity);
		}
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->bInvertYAxis = bInvertY;
		}
	}
}

void AZeroDawnSettingsMenu::ReturnToMainMenu()
{
	CloseMenu();
	UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyMap"));
}
