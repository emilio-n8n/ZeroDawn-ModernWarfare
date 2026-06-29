#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "../Character/ZeroDawnCharacter.h"
#include "ZeroDawnAISoldier.generated.h"

class UBehaviorTree;

UCLASS()
class AZeroDawnAISoldier : public AZeroDawnCharacter
{
	GENERATED_BODY()

public:
	AZeroDawnAISoldier(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Accuracy = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ReactionTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float FireRateAI = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 BurstSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float BurstCooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAIState InitialAIState = EAIState::Patrol;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void PerformAIShot();

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool HasLineOfSightToTarget() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetPatrolPath(const TArray<FVector>& Path);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FVector> PatrolPath;

protected:
	class AZeroDawnAIController* GetZeroDawnAIController() const;

	void AimAtTarget(float DeltaSeconds);

	FTimerHandle AIFireTimer;
	FTimerHandle AIBurstCooldownTimer;

	int32 CurrentBurstCount = 0;
	bool bCanAIFire = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float AISightRange = 4000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float AIFiringRange = 5000.0f;
};
