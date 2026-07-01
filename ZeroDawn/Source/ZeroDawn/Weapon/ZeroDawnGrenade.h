#pragma once

#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnGrenade.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AZeroDawnCharacter;

UCLASS()
class AZeroDawnGrenade : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnGrenade();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void LifeSpanExpired() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Root collision component for the grenade */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade")
	USphereComponent* CollisionComponent;

	/** Projectile movement component for physics-based travel */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade")
	UProjectileMovementComponent* ProjectileComponent;

	/** Visual mesh for the grenade */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade")
	UStaticMeshComponent* MeshComponent;

	/** Radius of the explosion in world units */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionRadius = 500.0f;

	/** Base damage dealt by the explosion */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionDamage = 200.0f;

	/** Time in seconds before the grenade explodes after being spawned */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionTimerDuration = 4.0f;

	/** Particle effect to spawn at explosion location */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	UParticleSystem* ExplosionEffect;

	/** Sound to play at explosion location */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	USoundCue* ExplosionSound;

	/**
	 * Called on all clients and server when the grenade explodes.
	 * Applies radial damage (server only) and spawns visual/audio effects.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExplode();
};
