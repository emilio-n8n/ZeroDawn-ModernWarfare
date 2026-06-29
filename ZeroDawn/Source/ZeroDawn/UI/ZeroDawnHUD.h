#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnHUD.generated.h"

class UUserWidget;

UCLASS()
class AZeroDawnHUD : public AHUD
{
	GENERATED_BODY()

public:
	AZeroDawnHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowScoreboard();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideScoreboard();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowKillFeed(const FString& KillerName, const FString& VictimName, EWeaponType Weapon);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateCrosshair(float Spread);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 CurrentAmmo, int32 ReserveAmmo);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateMatchTimer(const FString& TimeString);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateScore(int32 AlphaScore, int32 BravoScore);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ScoreboardWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> KillFeedWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* HUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* MenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* ScoreboardWidget;

protected:
	virtual void SetupWidgets();
};
