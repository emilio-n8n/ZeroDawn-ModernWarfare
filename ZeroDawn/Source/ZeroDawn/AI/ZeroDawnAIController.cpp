#include "ZeroDawnAIController.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "BehaviorTree/BlackboardData.h"

const FName ZeroDawnBlackboardKeys::TargetActorKey = TEXT("TargetActor");
const FName ZeroDawnBlackboardKeys::AIStateKey = TEXT("AIState");
const FName ZeroDawnBlackboardKeys::PatrolLocationKey = TEXT("PatrolLocation");
const FName ZeroDawnBlackboardKeys::LastKnownPositionKey = TEXT("LastKnownPosition");
const FName ZeroDawnBlackboardKeys::bIsInCombatKey = TEXT("bIsInCombat");
const FName ZeroDawnBlackboardKeys::bCanSeeTargetKey = TEXT("bCanSeeTarget");
const FName ZeroDawnBlackboardKeys::MoveToLocationKey = TEXT("MoveToLocation");

AZeroDawnAIController::AZeroDawnAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	bStartAILogicOnPossess = true;
}

void AZeroDawnAIController::BeginPlay()
{
	Super::BeginPlay();
	SetupPerceptionSystem();
}

void AZeroDawnAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree && Blackboard)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		RunBehaviorTree(BehaviorTree);
	}
}

void AZeroDawnAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentAIState == EAIState::Combat && CachedTarget)
	{
		if (LineOfSightTo(CachedTarget))
		{
			LastSeenTime = GetWorld()->GetTimeSeconds();
			BlackboardComp->SetValueAsBool(ZeroDawnBlackboardKeys::bCanSeeTargetKey, true);
			BlackboardComp->SetValueAsObject(ZeroDawnBlackboardKeys::TargetActorKey, CachedTarget);
			BlackboardComp->SetValueAsVector(ZeroDawnBlackboardKeys::LastKnownPositionKey, CachedTarget->GetActorLocation());
		}
		else
		{
			BlackboardComp->SetValueAsBool(ZeroDawnBlackboardKeys::bCanSeeTargetKey, false);
			float TimeSinceSeen = GetWorld()->GetTimeSeconds() - LastSeenTime;
			if (TimeSinceSeen >= LoseSightTime)
			{
				SetAIState(EAIState::Search);
			}
		}
	}
}

void AZeroDawnAIController::SetupPerceptionSystem()
{
	UAIPerceptionComponent* AIPerc = GetPerceptionComponent();
	if (!AIPerc) return;

	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = SightRange;
		SightConfig->LoseSightRadius = SightRange * 1.25f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightConfig->SetMaxAge(LoseSightTime);
		AIPerc->ConfigureSense(*SightConfig);
	}

	UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
		HearingConfig->SetMaxAge(3.0f);
		AIPerc->ConfigureSense(*HearingConfig);
	}

	AIPerc->OnTargetPerceptionUpdated.AddDynamic(this, &AZeroDawnAIController::OnTargetPerceptionUpdated);
	AIPerc->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

void AZeroDawnAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AZeroDawnCharacter* TargetChar = Cast<AZeroDawnCharacter>(Actor);
	if (!TargetChar) return;

	if (TargetChar->TeamType == ETeamType::None) return;

	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetPawn());
	if (OwnerChar && TargetChar->TeamType == OwnerChar->TeamType) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		CachedTarget = Actor;

		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			LastSeenTime = GetWorld()->GetTimeSeconds();
			SetTargetActor(Actor);
			SetAIState(EAIState::Combat);
		}
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			LastHeardTime = GetWorld()->GetTimeSeconds();
			if (CurrentAIState == EAIState::Patrol || CurrentAIState == EAIState::Idle)
			{
				SetAIState(EAIState::Alert);
				BlackboardComp->SetValueAsVector(ZeroDawnBlackboardKeys::LastKnownPositionKey, Stimulus.StimulusLocation);
				BlackboardComp->SetValueAsVector(ZeroDawnBlackboardKeys::MoveToLocationKey, Stimulus.StimulusLocation);
			}
		}
	}
}

void AZeroDawnAIController::SetTargetActor(AActor* Target)
{
	CachedTarget = Target;
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(ZeroDawnBlackboardKeys::TargetActorKey, Target);
		if (Target)
		{
			BlackboardComp->SetValueAsVector(ZeroDawnBlackboardKeys::LastKnownPositionKey, Target->GetActorLocation());
		}
	}
}

void AZeroDawnAIController::SetAIState(EAIState NewState)
{
	CurrentAIState = NewState;
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(ZeroDawnBlackboardKeys::AIStateKey, static_cast<uint8>(NewState));
		BlackboardComp->SetValueAsBool(ZeroDawnBlackboardKeys::bIsInCombatKey, NewState == EAIState::Combat);
	}
}

AActor* AZeroDawnAIController::GetTargetActor() const
{
	return CachedTarget;
}

EAIState AZeroDawnAIController::GetAIState() const
{
	return CurrentAIState;
}
