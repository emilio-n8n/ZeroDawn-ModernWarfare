#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnSettingsMenu.generated.h"

UCLASS()
class AZeroDawnSettingsMenu : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnSettingsMenu();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OpenMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OpenPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void CloseMenu();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplyGraphicsSettings(int32 QualityLevel, int32 ResolutionScale);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplyAudioSettings(float MasterVolume, float SFXVolume, float MusicVolume);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplyControlsSettings(float MouseSensitivity, float ADSMultiplier, bool bInvertY);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ReturnToMainMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> SettingsWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* CurrentMenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float MouseSensitivity = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float ADSMultiplier = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bInvertY = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float MasterVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float SFXVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float MusicVolume = 0.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	int32 GraphicsQuality = 3;

protected:
	APlayerController* GetPC() const;
};
