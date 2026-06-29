#include "ZeroDawnHitmarker.h"
#include "../Character/ZeroDawnCharacter.h"

UZeroDawnHitmarker::UZeroDawnHitmarker()
{
	SetIsReplicatedByDefault(true);
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
