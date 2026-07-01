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

void AZeroDawnBomb::PlantBomb()
{
	if (!HasAuthority()) return;
	if (bIsPlanted || bIsDefused) return;

	bIsCarried = false;
	bIsPlanted = true;
	bIsDefused = false;
	BombStartTime = GetWorld()->GetTimeSeconds();

	// Detach from any carrying actor and place at current location
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BombMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(true);

	OnRep_IsCarried();
	OnRep_IsPlanted();

	OnBombPlantedDelegate.Broadcast();
}

void AZeroDawnBomb::DefuseBomb()
{
	if (!HasAuthority()) return;
	if (!bIsPlanted || bIsDefused) return;

	bIsDefused = true;
	bIsPlanted = false;
	BombStartTime = 0.0f;

	OnRep_IsDefused();

	OnBombDefusedDelegate.Broadcast();
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

	MulticastExplode();

	OnBombExplodedDelegate.Broadcast();

	// Destroy the bomb after explosion
	Destroy();
}

void AZeroDawnBomb::ResetBomb()
{
	if (!HasAuthority()) return;

	bIsPlanted = false;
	bIsDefused = false;
	bIsCarried = false;
	BombStartTime = 0.0f;

	BombMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(true);

	// Cancel any pending timer
	GetWorldTimerManager().ClearTimer(BombTimerHandle);
}

void AZeroDawnBomb::Interact_Implementation(AActor* Interactor)
{
	// This handles defusing when the bomb is planted
	if (!bIsPlanted || bIsDefused) return;

	DefuseBomb();
}
