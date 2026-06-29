#include "ZeroDawnDoor.h"

AZeroDawnDoor::AZeroDawnDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AZeroDawnDoor::BeginPlay()
{
	Super::BeginPlay();
	ClosedRotation = GetActorRotation();
	OpenRotation = ClosedRotation + FRotator(0.0f, OpenAngle, 0.0f);
}

void AZeroDawnDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Target = bTargetOpen ? 1.0f : 0.0f;
	DoorInterp = FMath::FInterpTo(DoorInterp, Target, DeltaTime, OpenSpeed);

	FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, DoorInterp);
	SetActorRotation(NewRotation);
}

void AZeroDawnDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZeroDawnDoor, bIsOpen);
	DOREPLIFETIME(AZeroDawnDoor, bIsLocked);
}

void AZeroDawnDoor::ToggleDoor()
{
	ServerToggleDoor();
}

bool AZeroDawnDoor::ServerToggleDoor_Validate() { return true; }
void AZeroDawnDoor::ServerToggleDoor_Implementation()
{
	if (bIsLocked) return;

	if (bIsOpen) CloseDoor();
	else OpenDoor();
}

void AZeroDawnDoor::OpenDoor()
{
	bIsOpen = true;
	bTargetOpen = true;
}

void AZeroDawnDoor::CloseDoor()
{
	bIsOpen = false;
	bTargetOpen = false;
}
