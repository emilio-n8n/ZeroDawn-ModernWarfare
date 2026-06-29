#include "ZeroDawnBreakable.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

AZeroDawnBreakable::AZeroDawnBreakable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetNotifyRigidBodyCollision(true);
}

float AZeroDawnBreakable::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority()) return 0.0f;

	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		if (BreakEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BreakEffect, GetActorLocation());
		}

		if (BreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
		}

		OnBroken();

		if (bDestroyOnBreak)
		{
			Destroy();
		}
	}

	return DamageAmount;
}

void AZeroDawnBreakable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZeroDawnBreakable, Health);
}
