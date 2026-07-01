#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnHitmarker.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UZeroDawnHitmarker : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnHitmarker();

	/** Show a hitmarker on screen. Differentiates headshots (red tint) from body shots (white). */
	UFUNCTION(BlueprintCallable, Category = "Hitmarker")
	void ShowHitmarker(bool bHeadshot);

	/** Show kill confirmation visual feedback. */
	UFUNCTION(BlueprintCallable, Category = "Hitmarker")
	void ShowKillConfirmation();

	/** RPC: multicast hitmarker to all clients (only displays on the local client). */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHitmarker(bool bHeadshot);

	/** RPC: multicast kill confirmation to all clients (only displays on the local client). */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastKillConfirmation();

	/** Widget class for the hitmarker visual overlay. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	TSubclassOf<UUserWidget> HitmarkerWidgetClass;

	/** Widget class for the kill confirmation visual overlay. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	TSubclassOf<UUserWidget> KillConfirmationWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	USoundCue* HitmarkerSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	USoundCue* HeadshotSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hitmarker")
	USoundCue* KillSound;

protected:
	/** Currently active hitmarker widget instance. */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentHitmarkerWidget;

	/** Currently active kill confirmation widget instance. */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentKillConfirmationWidget;

	/** Timer handle for auto-removing the hitmarker widget. */
	FTimerHandle HitmarkerTimerHandle;

	/** Timer handle for auto-removing the kill confirmation widget. */
	FTimerHandle KillConfirmationTimerHandle;

	/** Get the owning player controller for widget creation. */
	APlayerController* GetPlayerController() const;
};
