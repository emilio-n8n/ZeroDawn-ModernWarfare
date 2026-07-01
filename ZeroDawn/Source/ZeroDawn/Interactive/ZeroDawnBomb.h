#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnInteractable.h"
#include "ZeroDawnBomb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBombEvent);

UCLASS()
class ZERODAWN_API AZeroDawnBomb : public AActor, public IZeroDawnInteractable
{
	GENERATED_BODY()

public:
	AZeroDawnBomb();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ------------------------------------------------------------------
	// State Properties
	// ------------------------------------------------------------------
	UPROPERTY(ReplicatedUsing = OnRep_IsPlanted, BlueprintReadOnly, Category = "Bomb")
	bool bIsPlanted = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsDefused, BlueprintReadOnly, Category = "Bomb")
	bool bIsDefused = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsCarried, BlueprintReadOnly, Category = "Bomb")
	bool bIsCarried = false;

	UFUNCTION()
	void OnRep_IsPlanted();

	UFUNCTION()
	void OnRep_IsDefused();

	UFUNCTION()
	void OnRep_IsCarried();

	// ------------------------------------------------------------------
	// Configuration
	// ------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb|Config")
	float BombTimer = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb|Config")
	float PlantTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb|Config")
	float DefuseTime = 7.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb|Config")
	float ExplosionRadius = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb|Config")
	float ExplosionDamage = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb|Config")
	TSubclassOf<UDamageType> ExplosionDamageType;

	// ------------------------------------------------------------------
	// Components
	// ------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb|Components")
	USkeletalMeshComponent* BombMesh;

	/** Sphere component used as a trigger for proximity / pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb|Components")
	USphereComponent* InteractionSphere;

	// ------------------------------------------------------------------
	// Events (broadcast to game mode / UI)
	// ------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "Bomb")
	FBombEvent OnBombPlantedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Bomb")
	FBombEvent OnBombDefusedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Bomb")
	FBombEvent OnBombExplodedDelegate;

	// ------------------------------------------------------------------
	// Plant / Defuse / Explode
	// ------------------------------------------------------------------
	/** Begin planting the bomb (called when player interacts with bomb site while carrying bomb) */
	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void PlantBomb();

	/** Begin defusing the bomb (called when player interacts with planted bomb) */
	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void DefuseBomb();

	/** Immediately explode, dealing radial damage */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExplode();

	/** Called when the bomb timer reaches zero */
	UFUNCTION(BlueprintNativeEvent, Category = "Bomb")
	void OnExploded();
	virtual void OnExploded_Implementation();

	/** Reset bomb to initial state for a new round */
	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void ResetBomb();

	// ------------------------------------------------------------------
	// IZeroDawnInteractable
	// ------------------------------------------------------------------
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	FTimerHandle BombTimerHandle;
	float BombStartTime = 0.0f;
};
