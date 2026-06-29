#include "ZeroDawnAISoldier.h"
#include "ZeroDawnAIController.h"
#include "../Weapon/ZeroDawnWeapon.h"
#include "../WeaponSystem/ZeroDawnWeaponComponent.h"

AZeroDawnAISoldier::AZeroDawnAISoldier(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationYaw = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AZeroDawnAIController::StaticClass();

	TeamType = ETeamType::Bravo;
}

void AZeroDawnAISoldier::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (AICon)
	{
		AICon->PatrolPoints = PatrolPath;
		AICon->SetAIState(InitialAIState);

		if (BehaviorTreeAsset)
		{
			AICon->BehaviorTree = BehaviorTreeAsset;
		}
	}
}

void AZeroDawnAISoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (AICon && AICon->GetAIState() == EAIState::Combat)
	{
		AimAtTarget(DeltaTime);

		if (bCanAIFire && WeaponComponent)
		{
			WeaponComponent->StartFire();
			CurrentBurstCount++;

			if (CurrentBurstCount >= BurstSize)
			{
				WeaponComponent->StopFire();
				bCanAIFire = false;
				CurrentBurstCount = 0;

				GetWorldTimerManager().SetTimer(AIBurstCooldownTimer, [this]()
				{
					bCanAIFire = true;
				}, BurstCooldown, false);
			}
		}
	}
}

float AZeroDawnAISoldier::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (AICon && EventInstigator)
	{
		APawn* AttackerPawn = EventInstigator->GetPawn();
		if (AttackerPawn)
		{
			AICon->SetTargetActor(AttackerPawn);
			AICon->SetAIState(EAIState::Combat);
		}
	}

	return ActualDamage;
}

AZeroDawnAIController* AZeroDawnAISoldier::GetZeroDawnAIController() const
{
	return Cast<AZeroDawnAIController>(GetController());
}

void AZeroDawnAISoldier::PerformAIShot()
{
	if (!WeaponComponent || !bCanAIFire) return;

	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (!AICon) return;

	AActor* Target = AICon->GetTargetActor();
	if (!Target || !HasLineOfSightToTarget()) return;

	FVector AimLocation = Target->GetActorLocation() + FVector(0, 0, 64);
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FRotator AimRotation = (AimLocation - Start).Rotation();

	GetController()->SetControlRotation(AimRotation);

	WeaponComponent->StartFire();
}

bool AZeroDawnAISoldier::HasLineOfSightToTarget() const
{
	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (!AICon) return false;

	AActor* Target = AICon->GetTargetActor();
	if (!Target) return false;

	return AICon->LineOfSightTo(Target);
}

void AZeroDawnAISoldier::SetPatrolPath(const TArray<FVector>& Path)
{
	PatrolPath = Path;
	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (AICon)
	{
		AICon->PatrolPoints = Path;
	}
}

void AZeroDawnAISoldier::AimAtTarget(float DeltaSeconds)
{
	AZeroDawnAIController* AICon = GetZeroDawnAIController();
	if (!AICon) return;

	AActor* Target = AICon->GetTargetActor();
	if (!Target) return;

	FVector TargetLocation = Target->GetActorLocation() + FVector(0, 0, 64);

	float RandomOffset = (1.0f - Accuracy) * 100.0f;
	TargetLocation.X += FMath::FRandRange(-RandomOffset, RandomOffset);
	TargetLocation.Y += FMath::FRandRange(-RandomOffset, RandomOffset);
	TargetLocation.Z += FMath::FRandRange(-RandomOffset, RandomOffset);

	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FRotator TargetRotation = (TargetLocation - StartLocation).Rotation();

	FRotator CurrentRotation = GetControlRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 5.0f);
	GetController()->SetControlRotation(NewRotation);
}
