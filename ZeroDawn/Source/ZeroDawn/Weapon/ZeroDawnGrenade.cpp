#include "ZeroDawnGrenade.h"
#include "../Character/ZeroDawnCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

AZeroDawnGrenade::AZeroDawnGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// Create collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(12.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetSimulatePhysics(false);
	RootComponent = CollisionComponent;

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetRelativeLocation(FVector::ZeroVector);

	// Create projectile movement component
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->InitialSpeed = 1500.0f;
	ProjectileComponent->MaxSpeed = 1500.0f;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = true;
	ProjectileComponent->Bounciness = 0.3f;
	ProjectileComponent->Friction = 0.5f;
	ProjectileComponent->ProjectileGravityScale = 1.0f;

	// The grenade will explode when InitialLifeSpan expires (4 seconds)
	InitialLifeSpan = ExplosionTimerDuration;
}

void AZeroDawnGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AZeroDawnGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZeroDawnGrenade::LifeSpanExpired()
{
	// When the 4-second lifespan expires, trigger the explosion via multicast
	MulticastExplode();

	// Let the parent handle destruction
	Super::LifeSpanExpired();
}

void AZeroDawnGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AZeroDawnGrenade::MulticastExplode_Implementation()
{
	// Spawn visual/audio effects on all clients and server
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	// Only the server applies damage
	if (!HasAuthority())
	{
		return;
	}

	// Collect teammates to exclude from friendly fire
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this); // Always ignore self

	AActor* OwnerActor = GetOwner();
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(OwnerActor);

	// Perform overlap check to find all pawns in the explosion radius
	if (OwnerChar)
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(this);

		GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			GetActorLocation(),
			FQuat::Identity,
			FCollisionObjectQueryParams(ECC_Pawn),
			FCollisionShape::MakeSphere(ExplosionRadius),
			QueryParams
		);

		// Add actors on the same team to the ignore list (friendly fire prevention)
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* HitActor = Overlap.GetActor();
			if (HitActor && HitActor != this)
			{
				AZeroDawnCharacter* HitChar = Cast<AZeroDawnCharacter>(HitActor);
				if (HitChar && HitChar->TeamType == OwnerChar->TeamType)
				{
					IgnoreActors.Add(HitActor);
				}
			}
		}
	}

	// Determine the instigator controller from the owning character
	AController* InstigatorController = nullptr;
	if (OwnerChar)
	{
		InstigatorController = OwnerChar->GetController();
	}

	// Apply radial damage only to non-teammates
	UGameplayStatics::ApplyRadialDamage(
		this,                                   // WorldContextObject
		ExplosionDamage,                         // BaseDamage
		GetActorLocation(),                      // Origin
		ExplosionRadius,                         // DamageRadius
		nullptr,                                 // DamageTypeClass (use default)
		IgnoreActors,                            // IgnoreActors (teammates + self)
		this,                                    // DamageCauser
		InstigatorController,                    // EventInstigator
		ECC_Visibility                           // DamagePreventionChannel
	);

}
