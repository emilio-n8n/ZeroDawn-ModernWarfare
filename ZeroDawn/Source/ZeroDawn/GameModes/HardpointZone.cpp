#include "HardpointZone.h"
#include "../Character/ZeroDawnCharacter.h"
#include "Components/BoxComponent.h"

AHardpointZone::AHardpointZone()
{
	PrimaryActorTick.bCanEverTick = true;

	ZoneTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneTrigger"));
	RootComponent = ZoneTrigger;
	ZoneTrigger->SetBoxExtent(FVector(200.0f));
	ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ZoneTrigger->SetGenerateOverlapEvents(true);

	bReplicates = true;
}

void AHardpointZone::BeginPlay()
{
	Super::BeginPlay();

	if (ZoneTrigger)
	{
		ZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHardpointZone::OnZoneBeginOverlap);
		ZoneTrigger->OnComponentEndOverlap.AddDynamic(this, &AHardpointZone::OnZoneEndOverlap);
	}
}

void AHardpointZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() || !bIsActive) return;

	PlayersInZone.RemoveAll([](AZeroDawnCharacter* Char) { return !Char || !Char->IsAlive(); });

	int32 AlphaCount = 0, BravoCount = 0;

	for (AZeroDawnCharacter* Char : PlayersInZone)
	{
		if (Char->TeamType == ETeamType::Alpha) AlphaCount++;
		else if (Char->TeamType == ETeamType::Bravo) BravoCount++;
	}

	if (AlphaCount > BravoCount) ControllingTeam = ETeamType::Alpha;
	else if (BravoCount > AlphaCount) ControllingTeam = ETeamType::Bravo;
	else ControllingTeam = ETeamType::None;
}

void AHardpointZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHardpointZone, bIsActive);
	DOREPLIFETIME(AHardpointZone, ControllingTeam);
}

void AHardpointZone::SetActive(bool bActive)
{
	bIsActive = bActive;
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);

	if (bActive)
	{
		ControllingTeam = ETeamType::None;
		PlayersInZone.Empty();
	}
}

void AHardpointZone::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;
	AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OtherActor);
	if (Char && Char->IsAlive())
	{
		PlayersInZone.AddUnique(Char);
	}
}

void AHardpointZone::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OtherActor);
	if (Char)
	{
		PlayersInZone.Remove(Char);
	}
}
