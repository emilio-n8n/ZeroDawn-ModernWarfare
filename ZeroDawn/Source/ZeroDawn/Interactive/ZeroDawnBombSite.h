#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnInteractable.h"
#include "ZeroDawnBombSite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBombSiteOverlap, AActor*, OverlappingActor);

UCLASS()
class ZERODAWN_API AZeroDawnBombSite : public AActor, public IZeroDawnInteractable
{
	GENERATED_BODY()

public:
	AZeroDawnBombSite();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ------------------------------------------------------------------
	// Configuration
	// ------------------------------------------------------------------
	/** Name of this bomb site: "A" or "B" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BombSite")
	FName SiteName;

	/** Team currently in control of this site (tracked by proximity) */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "BombSite")
	ETeamType TeamControlling;

	/** If true, this site has a planted bomb */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "BombSite")
	bool bHasPlantedBomb = false;

	// ------------------------------------------------------------------
	// Components
	// ------------------------------------------------------------------
	/** The trigger volume that detects player overlap */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombSite|Components")
	UBoxComponent* TriggerVolume;

	/** Visual representation of the site area (optional, can be a floor decal or plane) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombSite|Components")
	UStaticMeshComponent* SiteMesh;

	// ------------------------------------------------------------------
	// Overlap Events
	// ------------------------------------------------------------------
	/** Called when a player enters the bomb site trigger volume */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	/** Called when a player leaves the bomb site trigger volume */
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// ------------------------------------------------------------------
	// Delegates (broadcast to game mode / HUD)
	// ------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "BombSite")
	FBombSiteOverlap OnPlayerEnter;

	UPROPERTY(BlueprintAssignable, Category = "BombSite")
	FBombSiteOverlap OnPlayerExit;

	// ------------------------------------------------------------------
	// IZeroDawnInteractable
	// ------------------------------------------------------------------
	/** Handles plant interaction when player is near the bomb site carrying the bomb */
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	/** Set of players currently overlapping this trigger zone */
	TArray<TWeakObjectPtr<AActor>> OverlappingPlayers;

	/** Update team control based on which players are in the zone */
	void UpdateTeamControl();
};
