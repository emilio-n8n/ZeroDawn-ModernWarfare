#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "HardpointZone.generated.h"

class UBoxComponent;

UCLASS()
class AHardpointZone : public AActor
{
	GENERATED_BODY()

public:
	AHardpointZone();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Hardpoint")
	bool bIsActive = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Hardpoint")
	ETeamType ControllingTeam = ETeamType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardpoint")
	UBoxComponent* ZoneTrigger;

	UFUNCTION(BlueprintCallable, Category = "Hardpoint")
	void SetActive(bool bActive);

	UFUNCTION()
	void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<class AZeroDawnCharacter*> PlayersInZone;
};
