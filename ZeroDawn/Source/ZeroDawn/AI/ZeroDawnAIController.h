#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnAIController.generated.h"

UCLASS()
class AZeroDawnAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZeroDawnAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetAIState(EAIState NewState);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	EAIState GetAIState() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float SightRange = 4000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float HearingRange = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float LoseSightTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float CombatRadius = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bLoopPatrol = true;

protected:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void SetupPerceptionSystem();

	EAIState CurrentAIState = EAIState::Patrol;

	AActor* CachedTarget = nullptr;
	float LastSeenTime = 0.0f;
	float LastHeardTime = 0.0f;
	int32 CurrentPatrolIndex = 0;
};

namespace ZeroDawnBlackboardKeys
{
	extern const FName TargetActorKey;
	extern const FName AIStateKey;
	extern const FName PatrolLocationKey;
	extern const FName LastKnownPositionKey;
	extern const FName bIsInCombatKey;
	extern const FName bCanSeeTargetKey;
	extern const FName MoveToLocationKey;
}
