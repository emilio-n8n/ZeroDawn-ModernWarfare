#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnCreateAClass.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UZeroDawnCreateAClass : public UActorComponent
{
	GENERATED_BODY()

public:
	UZeroDawnCreateAClass();

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void SetLoadout(const FLoadoutClass& NewLoadout);

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	FLoadoutClass GetCurrentLoadout() const { return CurrentLoadout; }

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void SetPrimaryWeapon(EWeaponType Weapon);

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void SetSecondaryWeapon(EWeaponType Weapon);

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void SetPerk(EPerkSlot Slot, EPerkType Perk);

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void AddAttachment(EWeaponType ForWeapon, const FWeaponAttachment& Attachment);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Create-a-Class")
	TArray<FLoadoutClass> SavedLoadouts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Create-a-Class")
	FLoadoutClass CurrentLoadout;

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void SaveLoadout(int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Create-a-Class")
	void LoadLoadout(int32 Slot);
};
