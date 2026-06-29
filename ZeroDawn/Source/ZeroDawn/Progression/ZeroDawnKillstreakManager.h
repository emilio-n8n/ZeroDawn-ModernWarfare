#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnKillstreakManager.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnKillstreakManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnKillstreakManager();

	UFUNCTION(BlueprintCallable, Category = "Killstreaks")
	void CheckKillstreaks(int32 CurrentKillstreak);

	UFUNCTION(BlueprintCallable, Category = "Killstreaks")
	void RequestKillstreak(EKillstreakType Type);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Killstreaks")
	TMap<EKillstreakType, int32> KillstreakRequirements;

	UFUNCTION(BlueprintImplementableEvent, Category = "Killstreaks")
	void OnKillstreakAwarded(EKillstreakType Type);
};
