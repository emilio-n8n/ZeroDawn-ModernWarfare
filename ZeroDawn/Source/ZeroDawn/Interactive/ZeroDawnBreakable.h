#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnBreakable.generated.h"

UCLASS()
class AZeroDawnBreakable : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnBreakable();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
	bool bDestroyOnBreak = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Breakable")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
	UParticleSystem* BreakEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
	USoundCue* BreakSound;

	UFUNCTION(BlueprintImplementableEvent, Category = "Breakable")
	void OnBroken();
};
