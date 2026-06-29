#include "DominationZone.h"
#include "../Character/ZeroDawnCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

ADominationZone::ADominationZone()
{
	PrimaryActorTick.bCanEverTick = true;

	ZoneTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneTrigger"));
	RootComponent = ZoneTrigger;
	ZoneTrigger->SetBoxExtent(FVector(CaptureRadius));
	ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	ZoneTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ZoneTrigger->SetGenerateOverlapEvents(true);

	ZoneWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ZoneWidget"));
	ZoneWidget->SetupAttachment(RootComponent);

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ADominationZone::BeginPlay()
{
	Super::BeginPlay();
	ControllingTeam = DefaultOwner;

	if (ZoneTrigger)
	{
		ZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADominationZone::OnZoneBeginOverlap);
		ZoneTrigger->OnComponentEndOverlap.AddDynamic(this, &ADominationZone::OnZoneEndOverlap);
	}
}

void ADominationZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	CapturingPlayers.RemoveAll([](AZeroDawnCharacter* Char) { return !Char || !Char->IsAlive(); });

	if (CapturingPlayers.Num() == 0) return;

	int32 AlphaCount = 0, BravoCount = 0;
	for (AZeroDawnCharacter* Char : CapturingPlayers)
	{
		if (Char->TeamType == ETeamType::Alpha) AlphaCount++;
		else if (Char->TeamType == ETeamType::Bravo) BravoCount++;
	}

	ETeamType AttackingTeam = ETeamType::None;
	if (AlphaCount > BravoCount) AttackingTeam = ETeamType::Alpha;
	else if (BravoCount > AlphaCount) AttackingTeam = ETeamType::Bravo;

	if (AttackingTeam == ETeamType::None) return;

	if (AttackingTeam != ControllingTeam)
	{
		CaptureProgress += DeltaTime / CaptureTime;
		if (CaptureProgress >= 1.0f)
		{
			ControllingTeam = AttackingTeam;
			CaptureProgress = 0.0f;
		}
	}
}

void ADominationZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADominationZone, ControllingTeam);
	DOREPLIFETIME(ADominationZone, CaptureProgress);
}

void ADominationZone::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OtherActor);
	if (Char && Char->IsAlive())
	{
		CapturingPlayers.AddUnique(Char);
	}
}

void ADominationZone::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(OtherActor);
	if (Char)
	{
		CapturingPlayers.Remove(Char);
	}
}

bool ADominationZone::IsPlayerInZone(AZeroDawnCharacter* Character) const
{
	return CapturingPlayers.Contains(Character);
}
