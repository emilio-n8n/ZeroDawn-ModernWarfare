#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnWeaponInspect.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnWeaponInspect : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnWeaponInspect();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void StartInspect();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void StopInspect();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inspect")
	bool bIsInspecting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inspect")
	float InspectDuration = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inspect")
	float InspectMoveSpeed = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inspect")
	float InspectPitchAngle = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inspect")
	float InspectYawAngle = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inspect")
	float InspectFOVZoom = 45.0f;

	float InspectTimer = 0.0f;
	FRotator OriginalRotation;
	float OriginalFOV;
	APlayerController* OwnerPC;
	class UCameraComponent* OwnerCamera;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInspect();

	void PlayInspectMontage();
};
