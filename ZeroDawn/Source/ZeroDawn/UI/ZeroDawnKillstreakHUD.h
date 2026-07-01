#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnKillstreakHUD.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UZeroDawnKillstreakHUD : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnKillstreakHUD();

	/** Show a full-screen killstreak notification with its type and display name. */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	void ShowKillstreakNotification(EKillstreakType Type, const FString& StreakName);

	/** Show progress toward the next awardable killstreak (e.g. "3/5 kills"). */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	void ShowKillstreakProgress(int32 CurrentKills, int32 NextStreakKills);

	/** Display the current consecutive-kill streak counter. */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	void ShowStreakCounter(int32 CurrentStreak);

	/** Play the sound associated with a specific killstreak type. */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	void PlayKillstreakSound(EKillstreakType Type);

	/** Show the nuke countdown warning overlay with remaining seconds. */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	void ShowNukeWarning(int32 SecondsUntilNuke);

	/** Handle nuke detonation — clear warning UI and play detonation effects. */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	void OnNukeDetonated();

	/** Client RPC: show streak counter on the owning client. */
	UFUNCTION(Client, Reliable)
	void ClientShowStreak(int32 CurrentKillstreak);

	/** Client RPC: handle nuke detonation on the owning client. */
	UFUNCTION(Client, Reliable)
	void ClientNukeDetonated();

	/** Get the human-readable display name for a killstreak type. */
	UFUNCTION(BlueprintCallable, Category = "Killstreak")
	FString GetStreakName(EKillstreakType Type) const;

	// --- Widget classes (assigned in Blueprint or C++ defaults) ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> KillstreakNotificationWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> KillstreakProgressWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> StreakCounterWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> NukeWarningWidgetClass;

	// --- Sound assets ---

	/** Sound played for regular streak milestones (every 5 kills). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* StreakCounterSound;

	/** Sound played during nuke countdown warning. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* NukeWarningSound;

	/** Sound played when the nuke detonates. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* NukeDetonationSound;

	/** Per-type sound map used by PlayKillstreakSound. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TMap<EKillstreakType, TObjectPtr<USoundCue>> KillstreakSounds;

protected:
	/** Currently active notification widget. */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentNotificationWidget;

	/** Currently active progress widget. */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentProgressWidget;

	/** Currently active streak counter widget. */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentStreakCounterWidget;

	/** Currently active nuke warning widget. */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentNukeWarningWidget;

	/** Timer handle for auto-removing the notification widget. */
	FTimerHandle NotificationTimerHandle;

	/** Timer handle for auto-removing the progress widget. */
	FTimerHandle ProgressTimerHandle;

	/** Timer handle for auto-removing the streak counter widget. */
	FTimerHandle StreakCounterTimerHandle;

	/** Timer handle for auto-removing the nuke warning widget. */
	FTimerHandle NukeWarningTimerHandle;

	/** Get the owning player controller for widget creation. */
	APlayerController* GetPlayerController() const;
};
