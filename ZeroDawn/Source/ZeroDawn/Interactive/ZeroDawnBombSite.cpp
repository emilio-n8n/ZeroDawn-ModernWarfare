#include "ZeroDawnBombSite.h"
#include "ZeroDawnBomb.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"

AZeroDawnBombSite::AZeroDawnBombSite()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Create the trigger volume
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	SetRootComponent(TriggerVolume);
	TriggerVolume->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerVolume->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	TriggerVolume->SetGenerateOverlapEvents(true);

	// Optional visual mesh for the site area
	SiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SiteMesh"));
	SiteMesh->SetupAttachment(TriggerVolume);
	SiteMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZeroDawnBombSite::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AZeroDawnBombSite::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AZeroDawnBombSite::OnOverlapEnd);

	TeamControlling = ETeamType::None;
	PlantingBomb = nullptr;
}

void AZeroDawnBombSite::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZeroDawnBombSite, TeamControlling);
	DOREPLIFETIME(AZeroDawnBombSite, bHasPlantedBomb);
	DOREPLIFETIME(AZeroDawnBombSite, SiteName);
}

void AZeroDawnBombSite::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AZeroDawnCharacter* Character = Cast<AZeroDawnCharacter>(OtherActor);
	if (!Character) return;

	// Track this player
	OverlappingPlayers.Add(Character);

	OnPlayerEnter.Broadcast(Character);

	UpdateTeamControl();
}

void AZeroDawnBombSite::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AZeroDawnCharacter* Character = Cast<AZeroDawnCharacter>(OtherActor);
	if (!Character) return;

	// Remove from tracking
	OverlappingPlayers.Remove(Character);

	OnPlayerExit.Broadcast(Character);

	UpdateTeamControl();
}

void AZeroDawnBombSite::UpdateTeamControl()
{
	if (!HasAuthority()) return;

	// Determine controlling team by counting players in the zone
	int32 AlphaCount = 0;
	int32 BravoCount = 0;

	for (const TWeakObjectPtr<AActor>& Player : OverlappingPlayers)
	{
		if (!Player.IsValid()) continue;

		AZeroDawnCharacter* Character = Cast<AZeroDawnCharacter>(Player.Get());
		if (!Character) continue;

		AController* Controller = Character->GetController();
		if (!Controller) continue;

		AZeroDawnPlayerState* PS = Controller->GetPlayerState<AZeroDawnPlayerState>();
		if (!PS) continue;

		if (PS->PlayerTeam == ETeamType::Alpha)
			AlphaCount++;
		else if (PS->PlayerTeam == ETeamType::Bravo)
			BravoCount++;
	}

	if (AlphaCount > BravoCount)
		TeamControlling = ETeamType::Alpha;
	else if (BravoCount > AlphaCount)
		TeamControlling = ETeamType::Bravo;
	else
		TeamControlling = ETeamType::None;
}

void AZeroDawnBombSite::Interact_Implementation(AActor* Interactor)
{
	// Called on the owning client via line trace; forwards to server RPC
	ServerStartPlant(Interactor);
}

bool AZeroDawnBombSite::ServerStartPlant_Validate(AActor* Interactor) { return true; }
void AZeroDawnBombSite::ServerStartPlant_Implementation(AActor* Interactor)
{
	if (!HasAuthority()) return;

	// If a plant is already in progress, cancel it (toggle off)
	if (PlantingBomb)
	{
		CancelPlant();
		return;
	}

	// Check if the interacting player is carrying a bomb
	AZeroDawnBomb* CarriedBomb = GetCarriedBomb(Interactor);
	if (!CarriedBomb) return;
	if (CarriedBomb->bIsPlanted || CarriedBomb->bIsDefused) return;

	// Start the plant hold timer using the bomb's PlantTime
	PlantingBomb = CarriedBomb;
	float PlantDuration = CarriedBomb->PlantTime;

	GetWorldTimerManager().SetTimer(PlantTimerHandle, this, &AZeroDawnBombSite::CompletePlant, PlantDuration, false);
}

void AZeroDawnBombSite::CancelPlant()
{
	if (!HasAuthority()) return;

	GetWorldTimerManager().ClearTimer(PlantTimerHandle);
	PlantingBomb = nullptr;
}

void AZeroDawnBombSite::CompletePlant()
{
	if (!HasAuthority()) return;

	if (PlantingBomb)
	{
		// Verify the planter is still near the site
		bool bPlanterNearby = false;
		AActor* Planter = PlantingBomb->CarriedByCharacter;
		if (Planter)
		{
			for (const TWeakObjectPtr<AActor>& Player : OverlappingPlayers)
			{
				if (Player.IsValid() && Player.Get() == Planter)
				{
					bPlanterNearby = true;
					break;
				}
			}
		}

		if (bPlanterNearby)
		{
			bHasPlantedBomb = true;
			PlantingBomb->PlantBomb(Planter);
		}
	}

	PlantingBomb = nullptr;
}

AZeroDawnBomb* AZeroDawnBombSite::GetCarriedBomb(AActor* Character)
{
	// Find the bomb being carried by this character
	for (TActorIterator<AZeroDawnBomb> It(GetWorld()); It; ++It)
	{
		AZeroDawnBomb* Bomb = *It;
		if (Bomb && Bomb->bIsCarried && Bomb->CarriedByCharacter == Character)
		{
			return Bomb;
		}
	}
	return nullptr;
}
