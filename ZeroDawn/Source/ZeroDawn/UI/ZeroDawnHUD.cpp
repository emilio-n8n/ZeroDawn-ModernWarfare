#include "ZeroDawnHUD.h"
#include "ZeroDawnHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "../Multiplayer/ZeroDawnGameState.h"
#include "../Character/ZeroDawnCharacter.h"
#include "GameFramework/PlayerController.h"

AZeroDawnHUD::AZeroDawnHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

void AZeroDawnHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetOwningPlayerController()) && GetOwningPlayerController()->IsLocalController())
	{
		SetupWidgets();
	}
}

void AZeroDawnHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw dynamic crosshair based on weapon spread
	if (CrosshairSpread > 0.0f && Canvas)
	{
		const float CenterX = Canvas->SizeX * 0.5f;
		const float CenterY = Canvas->SizeY * 0.5f;
		const float LineLength = 12.0f;
		const float Gap = 4.0f + CrosshairSpread * 20.0f;
		const float Thickness = 1.5f;
		const FLinearColor CrosshairColor = FLinearColor::White;

		// Left line
		DrawLine(CenterX - Gap - LineLength, CenterY, CenterX - Gap, CenterY, CrosshairColor, Thickness);
		// Right line
		DrawLine(CenterX + Gap, CenterY, CenterX + Gap + LineLength, CenterY, CrosshairColor, Thickness);
		// Top line
		DrawLine(CenterX, CenterY - Gap - LineLength, CenterX, CenterY - Gap, CrosshairColor, Thickness);
		// Bottom line
		DrawLine(CenterX, CenterY + Gap, CenterX, CenterY + Gap + LineLength, CrosshairColor, Thickness);

		// Center dot
		DrawLine(CenterX - 1.0f, CenterY - 1.0f, CenterX + 1.0f, CenterY + 1.0f, CrosshairColor, Thickness);
		DrawLine(CenterX - 1.0f, CenterY + 1.0f, CenterX + 1.0f, CenterY - 1.0f, CrosshairColor, Thickness);
	}
	else
	{
		// Default crosshair when spread is 0 (not aiming / default state)
		const float CenterX = Canvas ? Canvas->SizeX * 0.5f : 0.0f;
		const float CenterY = Canvas ? Canvas->SizeY * 0.5f : 0.0f;
		if (Canvas)
		{
			const float DefaultGap = 6.0f;
			const float LineLength = 10.0f;
			const float Thickness = 1.5f;
			const FLinearColor CrosshairColor = FLinearColor::White;

			DrawLine(CenterX - DefaultGap - LineLength, CenterY, CenterX - DefaultGap, CenterY, CrosshairColor, Thickness);
			DrawLine(CenterX + DefaultGap, CenterY, CenterX + DefaultGap + LineLength, CenterY, CrosshairColor, Thickness);
			DrawLine(CenterX, CenterY - DefaultGap - LineLength, CenterX, CenterY - DefaultGap, CrosshairColor, Thickness);
			DrawLine(CenterX, CenterY + DefaultGap, CenterX, CenterY + DefaultGap + LineLength, CrosshairColor, Thickness);

			// Center dot
			DrawLine(CenterX - 1.0f, CenterY - 1.0f, CenterX + 1.0f, CenterY + 1.0f, CrosshairColor, Thickness);
			DrawLine(CenterX - 1.0f, CenterY + 1.0f, CenterX + 1.0f, CenterY - 1.0f, CrosshairColor, Thickness);
		}
	}
}

void AZeroDawnHUD::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AZeroDawnHUD::SetupWidgets()
{
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport(0);
		}
	}

	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MenuWidgetClass);
	}

	if (ScoreboardWidgetClass)
	{
		ScoreboardWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), ScoreboardWidgetClass);
	}
}

void AZeroDawnHUD::ShowHUDWidget()
{
	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport(0);
	}
}

void AZeroDawnHUD::HideHUDWidget()
{
	if (HUDWidget && HUDWidget->IsInViewport())
	{
		HUDWidget->RemoveFromParent();
	}
}

void AZeroDawnHUD::ShowMenu()
{
	APlayerController* PC = GetOwningPlayerController();
	if (PC && MenuWidget)
	{
		MenuWidget->AddToViewport(100);
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
		HideHUDWidget();
	}
}

void AZeroDawnHUD::HideMenu()
{
	APlayerController* PC = GetOwningPlayerController();
	if (PC && MenuWidget)
	{
		MenuWidget->RemoveFromParent();
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
		ShowHUDWidget();
	}
}

void AZeroDawnHUD::ShowScoreboard()
{
	if (ScoreboardWidget && !ScoreboardWidget->IsInViewport())
	{
		ScoreboardWidget->AddToViewport(50);
	}
}

void AZeroDawnHUD::HideScoreboard()
{
	if (ScoreboardWidget && ScoreboardWidget->IsInViewport())
	{
		ScoreboardWidget->RemoveFromParent();
	}
}

void AZeroDawnHUD::ShowKillFeed(const FString& KillerName, const FString& VictimName, EWeaponType Weapon)
{
	if (!KillFeedWidgetClass || !GetOwningPlayerController())
	{
		return;
	}

	UUserWidget* KillFeedEntry = CreateWidget<UUserWidget>(GetOwningPlayerController(), KillFeedWidgetClass);
	if (!KillFeedEntry)
	{
		return;
	}

	// Try to call a named "Setup" function on the kill feed entry widget via reflection
	UFunction* SetupFunc = KillFeedEntry->FindFunction(FName("Setup"));
	if (SetupFunc)
	{
		struct FKillFeedEntryParams
		{
			FString KillerName;
			FString VictimName;
			EWeaponType Weapon;
		};
		FKillFeedEntryParams Params;
		Params.KillerName = KillerName;
		Params.VictimName = VictimName;
		Params.Weapon = Weapon;
		KillFeedEntry->ProcessEvent(SetupFunc, &Params);
	}
	else
	{
		// Fallback: try individual setter functions
		UFunction* SetKillerFunc = KillFeedEntry->FindFunction(FName("SetKillerName"));
		if (SetKillerFunc)
		{
			KillFeedEntry->ProcessEvent(SetKillerFunc, (void*)&KillerName);
		}
		UFunction* SetVictimFunc = KillFeedEntry->FindFunction(FName("SetVictimName"));
		if (SetVictimFunc)
		{
			KillFeedEntry->ProcessEvent(SetVictimFunc, (void*)&VictimName);
		}
	}

	KillFeedEntry->AddToViewport(100);

	// Schedule auto-removal after 4 seconds with a member-tracked timer handle
	FTimerHandle& Handle = KillFeedTimerHandles.AddDefaulted_GetRef();
	TWeakObjectPtr<UUserWidget> WeakEntry(KillFeedEntry);
	TWeakObjectPtr<AZeroDawnHUD> WeakThis(this);
	GetWorldTimerManager().SetTimer(Handle, [WeakEntry, WeakThis]()
	{
		if (WeakEntry.IsValid() && WeakEntry->IsInViewport())
		{
			WeakEntry->RemoveFromParent();
		}
		if (WeakThis.IsValid())
		{
			WeakThis->KillFeedTimerHandles.RemoveAll([](const FTimerHandle& H) { return !H.IsValid(); });
		}
	}, 4.0f, false);
}

void AZeroDawnHUD::UpdateCrosshair(float Spread)
{
	CrosshairSpread = FMath::Clamp(Spread, 0.0f, 1.0f);
}

void AZeroDawnHUD::UpdateHealth(float Current, float Max)
{
	if (HUDWidget)
	{
		UZeroDawnHUDWidget* Widget = Cast<UZeroDawnHUDWidget>(HUDWidget);
		if (Widget)
		{
			Widget->UpdateHealthBar(Current, Max);
		}
	}
}

void AZeroDawnHUD::UpdateAmmo(int32 CurrentAmmo, int32 ReserveAmmo)
{
	if (HUDWidget)
	{
		UZeroDawnHUDWidget* Widget = Cast<UZeroDawnHUDWidget>(HUDWidget);
		if (Widget)
		{
			Widget->UpdateAmmoDisplay(CurrentAmmo, ReserveAmmo, 0);
		}
	}
}

void AZeroDawnHUD::UpdateMatchTimer(const FString& TimeString)
{
	if (HUDWidget)
	{
		UZeroDawnHUDWidget* Widget = Cast<UZeroDawnHUDWidget>(HUDWidget);
		if (Widget)
		{
			Widget->UpdateMatchTimer(TimeString);
		}
	}
}

void AZeroDawnHUD::UpdateScore(int32 AlphaScore, int32 BravoScore)
{
	if (HUDWidget)
	{
		UZeroDawnHUDWidget* Widget = Cast<UZeroDawnHUDWidget>(HUDWidget);
		if (Widget)
		{
			Widget->UpdateScoreDisplay(AlphaScore, BravoScore);
		}
	}
}

FString AZeroDawnHUD::GetWeaponDisplayName(EWeaponType Weapon) const
{
	switch (Weapon)
	{
	case EWeaponType::AssaultRifle:	return "Assault Rifle";
	case EWeaponType::Pistol:		return "Pistol";
	case EWeaponType::Shotgun:		return "Shotgun";
	case EWeaponType::SniperRifle:	return "Sniper Rifle";
	case EWeaponType::SMG:			return "SMG";
	case EWeaponType::Knife:		return "Knife";
	case EWeaponType::Grenade:		return "Grenade";
	default:						return "Unknown";
	}
}
