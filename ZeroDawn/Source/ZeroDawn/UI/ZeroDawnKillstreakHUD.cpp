#include "ZeroDawnKillstreakHUD.h"
#include "../Character/ZeroDawnCharacter.h"

UZeroDawnKillstreakHUD::UZeroDawnKillstreakHUD()
{
	SetIsReplicatedByDefault(true);
}

APlayerController* UZeroDawnKillstreakHUD::GetPlayerController() const
{
	AActor* Owner = GetOwner();
	if (!Owner) return nullptr;

	APawn* PawnOwner = Cast<APawn>(Owner);
	if (PawnOwner)
	{
		return Cast<APlayerController>(PawnOwner->GetController());
	}
	return nullptr;
}

void UZeroDawnKillstreakHUD::ShowKillstreakNotification(EKillstreakType Type, const FString& StreakName)
{
	// Remove any previous notification widget
	if (CurrentNotificationWidget && CurrentNotificationWidget->IsInViewport())
	{
		CurrentNotificationWidget->RemoveFromParent();
		CurrentNotificationWidget = nullptr;
	}

	if (!KillstreakNotificationWidgetClass) return;

	APlayerController* PC = GetPlayerController();
	if (!PC) return;

	CurrentNotificationWidget = CreateWidget<UUserWidget>(PC, KillstreakNotificationWidgetClass);
	if (!CurrentNotificationWidget) return;

	// Try to call a Setup(Type, StreakName) function on the widget
	UFunction* SetupFunc = CurrentNotificationWidget->FindFunction(FName("Setup"));
	if (SetupFunc)
	{
		struct FNotificationParams
		{
			EKillstreakType Type;
			FString StreakName;
		};
		FNotificationParams Params;
		Params.Type = Type;
		Params.StreakName = StreakName;
		CurrentNotificationWidget->ProcessEvent(SetupFunc, &Params);
	}
	else
	{
		// Fallback: try to set just the text
		UFunction* SetTextFunc = CurrentNotificationWidget->FindFunction(FName("SetText"));
		if (SetTextFunc)
		{
			CurrentNotificationWidget->ProcessEvent(SetTextFunc, (void*)&StreakName);
		}
	}

	CurrentNotificationWidget->AddToViewport(100);

	// Play the appropriate sound for this killstreak type
	PlayKillstreakSound(Type);

	// Auto-remove after 3 seconds
	TWeakObjectPtr<UUserWidget> WeakWidget(CurrentNotificationWidget);
	GetWorld()->GetTimerManager().SetTimer(NotificationTimerHandle, [WeakWidget]()
	{
		if (WeakWidget.IsValid() && WeakWidget->IsInViewport())
		{
			WeakWidget->RemoveFromParent();
		}
	}, 3.0f, false);
}

void UZeroDawnKillstreakHUD::ShowKillstreakProgress(int32 CurrentKills, int32 NextStreakKills)
{
	// Remove any previous progress widget
	if (CurrentProgressWidget && CurrentProgressWidget->IsInViewport())
	{
		CurrentProgressWidget->RemoveFromParent();
		CurrentProgressWidget = nullptr;
	}

	if (!KillstreakProgressWidgetClass) return;

	APlayerController* PC = GetPlayerController();
	if (!PC) return;

	CurrentProgressWidget = CreateWidget<UUserWidget>(PC, KillstreakProgressWidgetClass);
	if (!CurrentProgressWidget) return;

	// Try to call a SetProgress(CurrentKills, NextStreakKills) function on the widget
	UFunction* SetProgressFunc = CurrentProgressWidget->FindFunction(FName("SetProgress"));
	if (SetProgressFunc)
	{
		struct FProgressParams
		{
			int32 CurrentKills;
			int32 NextStreakKills;
		};
		FProgressParams Params;
		Params.CurrentKills = CurrentKills;
		Params.NextStreakKills = NextStreakKills;
		CurrentProgressWidget->ProcessEvent(SetProgressFunc, &Params);
	}

	CurrentProgressWidget->AddToViewport(100);

	// Auto-remove after 2 seconds
	TWeakObjectPtr<UUserWidget> WeakWidget(CurrentProgressWidget);
	GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, [WeakWidget]()
	{
		if (WeakWidget.IsValid() && WeakWidget->IsInViewport())
		{
			WeakWidget->RemoveFromParent();
		}
	}, 2.0f, false);
}

void UZeroDawnKillstreakHUD::ShowStreakCounter(int32 CurrentStreak)
{
	// Remove any previous streak counter widget
	if (CurrentStreakCounterWidget && CurrentStreakCounterWidget->IsInViewport())
	{
		CurrentStreakCounterWidget->RemoveFromParent();
		CurrentStreakCounterWidget = nullptr;
	}

	if (!StreakCounterWidgetClass) return;

	APlayerController* PC = GetPlayerController();
	if (!PC) return;

	CurrentStreakCounterWidget = CreateWidget<UUserWidget>(PC, StreakCounterWidgetClass);
	if (!CurrentStreakCounterWidget) return;

	// Try to call a SetStreak(int32) function on the widget
	UFunction* SetStreakFunc = CurrentStreakCounterWidget->FindFunction(FName("SetStreak"));
	if (SetStreakFunc)
	{
		CurrentStreakCounterWidget->ProcessEvent(SetStreakFunc, (void*)&CurrentStreak);
	}

	CurrentStreakCounterWidget->AddToViewport(100);

	// Auto-remove after 2 seconds
	TWeakObjectPtr<UUserWidget> WeakWidget(CurrentStreakCounterWidget);
	GetWorld()->GetTimerManager().SetTimer(StreakCounterTimerHandle, [WeakWidget]()
	{
		if (WeakWidget.IsValid() && WeakWidget->IsInViewport())
		{
			WeakWidget->RemoveFromParent();
		}
	}, 2.0f, false);
}

void UZeroDawnKillstreakHUD::PlayKillstreakSound(EKillstreakType Type)
{
	// Prefer per-type mapped sound if available
	if (KillstreakSounds.Contains(Type))
	{
		USoundCue* Sound = KillstreakSounds[Type];
		if (Sound)
		{
			UGameplayStatics::PlaySound2D(this, Sound);
			return;
		}
	}

	// Fallback: use type-specific defaults
	switch (Type)
	{
	case EKillstreakType::Nuke:
		if (NukeWarningSound)
		{
			UGameplayStatics::PlaySound2D(this, NukeWarningSound);
		}
		break;
	default:
		if (StreakCounterSound)
		{
			UGameplayStatics::PlaySound2D(this, StreakCounterSound);
		}
		break;
	}
}

void UZeroDawnKillstreakHUD::ShowNukeWarning(int32 SecondsUntilNuke)
{
	// Remove any previous nuke warning widget
	if (CurrentNukeWarningWidget && CurrentNukeWarningWidget->IsInViewport())
	{
		CurrentNukeWarningWidget->RemoveFromParent();
		CurrentNukeWarningWidget = nullptr;
	}

	if (!NukeWarningWidgetClass) return;

	APlayerController* PC = GetPlayerController();
	if (!PC) return;

	CurrentNukeWarningWidget = CreateWidget<UUserWidget>(PC, NukeWarningWidgetClass);
	if (!CurrentNukeWarningWidget) return;

	// Try to call a SetCountdown(int32) function on the widget
	UFunction* SetCountdownFunc = CurrentNukeWarningWidget->FindFunction(FName("SetCountdown"));
	if (SetCountdownFunc)
	{
		CurrentNukeWarningWidget->ProcessEvent(SetCountdownFunc, (void*)&SecondsUntilNuke);
	}

	CurrentNukeWarningWidget->AddToViewport(200); // Above HUD, very prominent

	// Play warning sound
	if (NukeWarningSound)
	{
		UGameplayStatics::PlaySound2D(this, NukeWarningSound);
	}

	// Keep the warning visible — it will be removed by OnNukeDetonated or a subsequent call
}

void UZeroDawnKillstreakHUD::OnNukeDetonated()
{
	// Remove any visible nuke warning overlay
	if (CurrentNukeWarningWidget && CurrentNukeWarningWidget->IsInViewport())
	{
		CurrentNukeWarningWidget->RemoveFromParent();
		CurrentNukeWarningWidget = nullptr;
	}

	// Play the detonation sound
	if (NukeDetonationSound)
	{
		UGameplayStatics::PlaySound2D(this, NukeDetonationSound);
	}

	// Camera shake is handled separately via UZeroDawnCameraShake
}

void UZeroDawnKillstreakHUD::ClientShowStreak_Implementation(int32 CurrentKillstreak)
{
	ShowStreakCounter(CurrentKillstreak);

	if (CurrentKillstreak % 5 == 0 && CurrentKillstreak > 0 && StreakCounterSound)
	{
		UGameplayStatics::PlaySound2D(this, StreakCounterSound);
	}
}

void UZeroDawnKillstreakHUD::ClientNukeDetonated_Implementation()
{
	if (NukeDetonationSound)
	{
		UGameplayStatics::PlaySound2D(this, NukeDetonationSound);
	}
	OnNukeDetonated();
}

FString UZeroDawnKillstreakHUD::GetStreakName(EKillstreakType Type) const
{
	switch (Type)
	{
	case EKillstreakType::UAV: return TEXT("UAV - Spy Plane");
	case EKillstreakType::CarePackage: return TEXT("Care Package");
	case EKillstreakType::AttackHelicopter: return TEXT("Attack Helicopter");
	case EKillstreakType::Gunship: return TEXT("Gunship - AC-130");
	case EKillstreakType::Nuke: return TEXT("TACTICAL NUKE INCOMING!");
	default: return TEXT("");
	}
}
