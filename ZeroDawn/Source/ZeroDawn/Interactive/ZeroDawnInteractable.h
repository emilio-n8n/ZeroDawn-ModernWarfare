#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UZeroDawnInteractable : public UInterface
{
	GENERATED_BODY()
};

class IZeroDawnInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);
};
