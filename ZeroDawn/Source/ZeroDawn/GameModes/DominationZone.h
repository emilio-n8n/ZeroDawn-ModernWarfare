#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "DominationZone.generated.h"

class UBoxComponent;
class UWidgetComponent;
class AZeroDawnCharacter;

UCLASS()
class ADominationZone : public AActor
{
	GENERATED_BODY()

public:
	ADominationZone();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString ZoneName = "A";

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	ETeamType ControllingTeam = ETeamType::None;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	float CaptureProgress = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone")
	float CaptureTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone")
	float CaptureRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone")
	ETeamType DefaultOwner = ETeamType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	TArray<AZeroDawnCharacter*> CapturingPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	UBoxComponent* ZoneTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	UWidgetComponent* ZoneWidget;

	UFUNCTION()
	void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsPlayerInZone(AZeroDawnCharacter* Character) const;
};
