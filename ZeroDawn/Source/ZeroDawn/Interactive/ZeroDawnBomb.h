#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnInteractable.h"
#include "ZeroDawnBomb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBombEvent);

class AZeroDawnCharacter;

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

	/** Which team planted the bomb */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	ETeamType PlantedByTeam = ETeamType::None;

	/** The character currently carrying this bomb */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	AZeroDawnCharacter* CarriedByCharacter = nullptr;

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
	void PlantBomb(AActor* Planter = nullptr);

	/** Start defuse hold interaction — calls DefuseBomb() after DefuseTime */
	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void StartDefuse(AActor* Defuser);

	/** Immediately defuse the bomb */
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

	/** Cancel any ongoing defuse interaction */
	void CancelDefuse();

	// ------------------------------------------------------------------
	// IZeroDawnInteractable
	// ------------------------------------------------------------------
	virtual void Interact_Implementation(AActor* Interactor) override;

	/** Server RPC: start defusing (called from Interact_Implementation on client) */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartDefuse(AActor* Interactor);

protected:
	FTimerHandle BombTimerHandle;
	FTimerHandle DefuseTimerHandle;
	float BombStartTime = 0.0f;

	/** The actor currently defusing (null if none) */
	AActor* InteractingDefuser = nullptr;

	/** Internal: complete the defuse after the hold timer */
	void CompleteDefuse();
};
