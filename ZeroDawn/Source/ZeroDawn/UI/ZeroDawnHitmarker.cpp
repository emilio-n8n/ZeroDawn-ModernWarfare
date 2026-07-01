#include "ZeroDawnHitmarker.h"
#include "../Character/ZeroDawnCharacter.h"

UZeroDawnHitmarker::UZeroDawnHitmarker()
{
	SetIsReplicatedByDefault(true);
}

APlayerController* UZeroDawnHitmarker::GetPlayerController() const
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

void UZeroDawnHitmarker::ShowHitmarker(bool bHeadshot)
{
	// Remove any previous hitmarker still visible
	if (CurrentHitmarkerWidget && CurrentHitmarkerWidget->IsInViewport())
	{
		CurrentHitmarkerWidget->RemoveFromParent();
		CurrentHitmarkerWidget = nullptr;
	}

	if (!HitmarkerWidgetClass) return;

	APlayerController* PC = GetPlayerController();
	if (!PC) return;

	CurrentHitmarkerWidget = CreateWidget<UUserWidget>(PC, HitmarkerWidgetClass);
	if (!CurrentHitmarkerWidget) return;

	// Notify the widget whether this is a headshot (Blueprint widgets can react to this)
	UFunction* SetHeadshotFunc = CurrentHitmarkerWidget->FindFunction(FName("SetHeadshot"));
	if (SetHeadshotFunc)
	{
		struct FSetHeadshotParams
		{
			bool bHeadshot;
		};
		FSetHeadshotParams Params;
		Params.bHeadshot = bHeadshot;
		CurrentHitmarkerWidget->ProcessEvent(SetHeadshotFunc, &Params);
	}

	CurrentHitmarkerWidget->AddToViewport(101); // Above standard HUD layer

	// Auto-remove the hitmarker after 0.3 seconds
	TWeakObjectPtr<UUserWidget> WeakWidget(CurrentHitmarkerWidget);
	GetWorld()->GetTimerManager().SetTimer(HitmarkerTimerHandle, [WeakWidget]()
	{
		if (WeakWidget.IsValid() && WeakWidget->IsInViewport())
		{
			WeakWidget->RemoveFromParent();
		}
	}, 0.3f, false);
}

void UZeroDawnHitmarker::ShowKillConfirmation()
{
	// Remove any previous kill confirmation still visible
	if (CurrentKillConfirmationWidget && CurrentKillConfirmationWidget->IsInViewport())
	{
		CurrentKillConfirmationWidget->RemoveFromParent();
		CurrentKillConfirmationWidget = nullptr;
	}

	if (!KillConfirmationWidgetClass) return;

	APlayerController* PC = GetPlayerController();
	if (!PC) return;

	CurrentKillConfirmationWidget = CreateWidget<UUserWidget>(PC, KillConfirmationWidgetClass);
	if (!CurrentKillConfirmationWidget) return;

	CurrentKillConfirmationWidget->AddToViewport(100); // HUD layer

	// Auto-remove the kill confirmation after 1.5 seconds
	TWeakObjectPtr<UUserWidget> WeakWidget(CurrentKillConfirmationWidget);
	GetWorld()->GetTimerManager().SetTimer(KillConfirmationTimerHandle, [WeakWidget]()
	{
		if (WeakWidget.IsValid() && WeakWidget->IsInViewport())
		{
			WeakWidget->RemoveFromParent();
		}
	}, 1.5f, false);
}

void UZeroDawnHitmarker::MulticastHitmarker_Implementation(bool bHeadshot)
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && !OwnerChar->IsLocallyControlled()) return;

	if (bHeadshot && HeadshotSound)
	{
		UGameplayStatics::PlaySound2D(this, HeadshotSound);
	}
	else if (HitmarkerSound)
	{
		UGameplayStatics::PlaySound2D(this, HitmarkerSound);
	}

	ShowHitmarker(bHeadshot);
}

void UZeroDawnHitmarker::MulticastKillConfirmation_Implementation()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && !OwnerChar->IsLocallyControlled()) return;

	if (KillSound)
	{
		UGameplayStatics::PlaySound2D(this, KillSound);
	}

	ShowKillConfirmation();
}
