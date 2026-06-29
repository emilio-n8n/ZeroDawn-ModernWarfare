#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnDoor.generated.h"

UCLASS()
class AZeroDawnDoor : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnDoor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenSpeed = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void ToggleDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleDoor();

protected:
	FRotator ClosedRotation;
	FRotator OpenRotation;
	float DoorInterp = 0.0f;
	bool bTargetOpen = false;
};
