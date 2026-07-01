#include "ZeroDawnBomb.h"
#include "../Character/ZeroDawnCharacter.h"
#include "Components/SphereComponent.h"

AZeroDawnBomb::AZeroDawnBomb()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Skeletal mesh as the visual representation
	BombMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BombMesh"));
	SetRootComponent(BombMesh);
	BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BombMesh->SetSimulatePhysics(false);

	// Interaction sphere for proximity checks
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(BombMesh);
	InteractionSphere->SetSphereRadius(120.0f);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AZeroDawnBomb::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BombStartTime = 0.0f;
		CarriedByCharacter = nullptr;
	}
}

void AZeroDawnBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	// Only tick the bomb timer when planted
	if (bIsPlanted && !bIsDefused)
	{
		float Elapsed = GetWorld()->GetTimeSeconds() - BombStartTime;
		if (Elapsed >= BombTimer)
		{
			// Explode
			OnExploded();
		}
	}
}

void AZeroDawnBomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZeroDawnBomb, bIsPlanted);
	DOREPLIFETIME(AZeroDawnBomb, bIsDefused);
	DOREPLIFETIME(AZeroDawnBomb, bIsCarried);
	DOREPLIFETIME(AZeroDawnBomb, PlantedByTeam);
	DOREPLIFETIME(AZeroDawnBomb, CarriedByCharacter);
}

void AZeroDawnBomb::OnRep_IsPlanted()
{
	// Clients respond to visual state change here
}

void AZeroDawnBomb::OnRep_IsDefused()
{
	// Clients respond to visual state change here
}

void AZeroDawnBomb::OnRep_IsCarried()
{
	// Clients respond to carry state: attach/detach from player
	if (bIsCarried)
	{
		BombMesh->SetSimulatePhysics(false);
	}
	else if (!bIsPlanted)
	{
		// Dropped on the ground (not planted, not carried)
		BombMesh->SetSimulatePhysics(true);
	}
}

void AZeroDawnBomb::PlantBomb(AActor* Planter)
{
	if (!HasAuthority()) return;
	if (bIsPlanted || bIsDefused) return;

	bIsCarried = false;
	bIsPlanted = true;
	bIsDefused = false;
	BombStartTime = GetWorld()->GetTimeSeconds();

	// Track who planted and which team
	if (Planter)
	{
		AZeroDawnCharacter* Char = Cast<AZeroDawnCharacter>(Planter);
		if (Char)
		{
			PlantedByTeam = Char->TeamType;
		}
	}

	CarriedByCharacter = nullptr;

	// Detach from any carrying actor and place at current location
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BombMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(true);

	OnRep_IsCarried();
	OnRep_IsPlanted();

	OnBombPlantedDelegate.Broadcast();
}

void AZeroDawnBomb::StartDefuse(AActor* Defuser)
{
	if (!HasAuthority()) return;
	if (!bIsPlanted || bIsDefused) return;
	if (InteractingDefuser) return; // already being defused

	InteractingDefuser = Defuser;

	// Start the defuse hold timer using DefuseTime
	GetWorldTimerManager().SetTimer(DefuseTimerHandle, this, &AZeroDawnBomb::CompleteDefuse, DefuseTime, false);
}

void AZeroDawnBomb::DefuseBomb()
{
	if (!HasAuthority()) return;
	if (!bIsPlanted || bIsDefused) return;

	bIsDefused = true;
	bIsPlanted = false;
	BombStartTime = 0.0f;

	// Cancel any ongoing defuse timer
	GetWorldTimerManager().ClearTimer(DefuseTimerHandle);
	InteractingDefuser = nullptr;

	OnRep_IsDefused();

	OnBombDefusedDelegate.Broadcast();
}

void AZeroDawnBomb::CompleteDefuse()
{
	if (!HasAuthority()) return;
	if (!bIsPlanted || bIsDefused) return;

	DefuseBomb();
}

void AZeroDawnBomb::CancelDefuse()
{
	if (!HasAuthority()) return;

	GetWorldTimerManager().ClearTimer(DefuseTimerHandle);
	InteractingDefuser = nullptr;
}

void AZeroDawnBomb::MulticastExplode_Implementation()
{
	if (!HasAuthority()) return;

	// Apply radial damage to all actors in range
	UGameplayStatics::ApplyRadialDamage(
		this,
		ExplosionDamage,
		GetActorLocation(),
		ExplosionRadius,
		ExplosionDamageType,
		TArray<AActor*>(),  // Ignore actors (none specifically)
		this,                 // Damage causer
		nullptr,              // Controller (no specific controller)
		false                 // Do full damage
	);
}

void AZeroDawnBomb::OnExploded_Implementation()
{
	if (!HasAuthority()) return;

	// Cancel any defuse in progress
	CancelDefuse();

	MulticastExplode();

	// Reset bomb state manually (don't call ResetBomb which unhides)
	bIsPlanted = false;
	bIsDefused = false;
	bIsCarried = false;
	PlantedByTeam = ETeamType::None;
	CarriedByCharacter = nullptr;
	BombStartTime = 0.0f;

	BombMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(false);

	// Hide the bomb visually after explosion
	SetActorHiddenInGame(true);

	OnBombExplodedDelegate.Broadcast();
}

void AZeroDawnBomb::ResetBomb()
{
	if (!HasAuthority()) return;

	bIsPlanted = false;
	bIsDefused = false;
	bIsCarried = false;
	PlantedByTeam = ETeamType::None;
	CarriedByCharacter = nullptr;
	BombStartTime = 0.0f;

	CancelDefuse();

	BombMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(true);

	// Unhide the bomb for the new round
	SetActorHiddenInGame(false);

	// Cancel any pending timer
	GetWorldTimerManager().ClearTimer(BombTimerHandle);
}

void AZeroDawnBomb::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	if (bIsPlanted && !bIsDefused)
	{
		// Bomb is planted → start defusing
		ServerStartDefuse(Interactor);
	}
	else if (!bIsPlanted && !bIsDefused && !bIsCarried)
	{
		// Bomb is on the ground → pick it up
		ServerPickup(Interactor);
	}
}

bool AZeroDawnBomb::ServerStartDefuse_Validate(AActor* Interactor) { return true; }
void AZeroDawnBomb::ServerStartDefuse_Implementation(AActor* Interactor)
{
	if (!HasAuthority()) return;
	if (!bIsPlanted || bIsDefused) return;

	StartDefuse(Interactor);
}

bool AZeroDawnBomb::ServerPickup_Validate(AActor* Interactor) { return true; }
void AZeroDawnBomb::ServerPickup_Implementation(AActor* Interactor)
{
	if (!HasAuthority()) return;
	if (bIsPlanted || bIsDefused || bIsCarried) return;
	if (!Interactor) return;

	AZeroDawnCharacter* Character = Cast<AZeroDawnCharacter>(Interactor);
	if (!Character) return;

	// Set carry state
	bIsCarried = true;
	CarriedByCharacter = Character;

	// Disable physics and collision while carried
	BombMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(false);

	// Attach to the character's mesh
	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
	if (CharacterMesh)
	{
		AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("BombSocket")));
	}
	else
	{
		AttachToActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	OnRep_IsCarried();
}

void AZeroDawnBomb::DropBomb()
{
	if (!HasAuthority()) return;
	if (!bIsCarried) return;

	bIsCarried = false;
	CarriedByCharacter = nullptr;

	// Detach from the carrier
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Re-enable physics and collision so the bomb falls to the ground
	BombMesh->SetSimulatePhysics(true);
	SetActorEnableCollision(true);

	OnRep_IsCarried();
}
