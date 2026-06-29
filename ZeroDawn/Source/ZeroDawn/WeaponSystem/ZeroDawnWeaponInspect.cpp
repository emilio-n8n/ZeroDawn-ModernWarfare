#include "ZeroDawnWeaponInspect.h"
#include "../Character/ZeroDawnCharacter.h"
#include "../Weapon/ZeroDawnWeapon.h"
#include "ZeroDawnWeaponComponent.h"
#include "Camera/CameraComponent.h"

UZeroDawnWeaponInspect::UZeroDawnWeaponInspect()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UZeroDawnWeaponInspect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsInspecting) return;

	InspectTimer += DeltaTime;

	if (InspectTimer >= InspectDuration)
	{
		StopInspect();
		return;
	}

	float Progress = FMath::Clamp(InspectTimer / InspectDuration, 0.0f, 1.0f);

	float Angle = FMath::Sin(Progress * PI) * InspectYawAngle;
	float Pitch = FMath::Sin(Progress * PI * 0.5f) * InspectPitchAngle;

	if (OwnerCamera)
	{
		float TargetFOV = FMath::Lerp(OriginalFOV, InspectFOVZoom, FMath::Sin(Progress * PI));
		OwnerCamera->FieldOfView = TargetFOV;
	}

	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && OwnerChar->FirstPersonCameraComponent)
	{
		FRotator NewRot = OriginalRotation;
		NewRot.Yaw += Angle;
		NewRot.Pitch += Pitch;
		OwnerChar->FirstPersonCameraComponent->SetRelativeRotation(NewRot);
	}
}

void UZeroDawnWeaponInspect::StartInspect()
{
	if (bIsInspecting) return;

	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->IsAlive()) return;

	bIsInspecting = true;
	InspectTimer = 0.0f;

	if (OwnerChar->FirstPersonCameraComponent)
	{
		OwnerCamera = OwnerChar->FirstPersonCameraComponent;
		OriginalRotation = OwnerCamera->GetRelativeRotation();
		OriginalFOV = OwnerCamera->FieldOfView;
	}

	SetComponentTickEnabled(true);
	PlayInspectMontage();
	ServerInspect();
}

void UZeroDawnWeaponInspect::StopInspect()
{
	if (!bIsInspecting) return;
	bIsInspecting = false;
	SetComponentTickEnabled(false);

	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (OwnerChar && OwnerChar->FirstPersonCameraComponent)
	{
		OwnerChar->FirstPersonCameraComponent->SetRelativeRotation(OriginalRotation);
		OwnerChar->FirstPersonCameraComponent->FieldOfView = OriginalFOV;
	}
}

bool UZeroDawnWeaponInspect::ServerInspect_Validate() { return true; }
void UZeroDawnWeaponInspect::ServerInspect_Implementation() {}

void UZeroDawnWeaponInspect::PlayInspectMontage()
{
	AZeroDawnCharacter* OwnerChar = Cast<AZeroDawnCharacter>(GetOwner());
	if (!OwnerChar) return;

	AZeroDawnWeapon* Weapon = OwnerChar->WeaponComponent
		? OwnerChar->WeaponComponent->GetCurrentWeapon() : nullptr;

	if (Weapon && Weapon->WeaponMesh && Weapon->WeaponMesh->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = Weapon->WeaponMesh->GetAnimInstance();
		AnimInstance->Montage_Play(nullptr);
	}
}
