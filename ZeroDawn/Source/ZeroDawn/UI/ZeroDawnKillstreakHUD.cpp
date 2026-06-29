#include "ZeroDawnKillstreakHUD.h"
#include "../Character/ZeroDawnCharacter.h"

UZeroDawnKillstreakHUD::UZeroDawnKillstreakHUD()
{
	SetIsReplicatedByDefault(true);
}

void UZeroDawnKillstreakHUD::MulticastShowStreak_Implementation(int32 CurrentKillstreak)
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && !OwnerChar->IsLocallyControlled()) return;

	ShowStreakCounter(CurrentKillstreak);

	if (CurrentKillstreak % 5 == 0 && CurrentKillstreak > 0 && StreakCounterSound)
	{
		UGameplayStatics::PlaySound2D(this, StreakCounterSound);
	}
}

void UZeroDawnKillstreakHUD::MulticastNukeDetonated_Implementation()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && !OwnerChar->IsLocallyControlled()) return;

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
