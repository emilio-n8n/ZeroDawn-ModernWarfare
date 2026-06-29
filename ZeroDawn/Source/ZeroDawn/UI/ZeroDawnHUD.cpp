#include "ZeroDawnHUD.h"
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

void AZeroDawnHUD::ShowKillFeed(const FString& KillerName, const FString& VictimName, EWeaponType Weapon) {}

void AZeroDawnHUD::UpdateCrosshair(float Spread) {}
void AZeroDawnHUD::UpdateHealth(float Current, float Max) {}
void AZeroDawnHUD::UpdateAmmo(int32 CurrentAmmo, int32 ReserveAmmo) {}
void AZeroDawnHUD::UpdateMatchTimer(const FString& TimeString) {}
void AZeroDawnHUD::UpdateScore(int32 AlphaScore, int32 BravoScore) {}
