#pragma once
#include "../ZeroDawn.h"
#include "ZeroDawnLevelGenerator.generated.h"

class AZeroDawnBombSite;

UCLASS(Blueprintable)
class AZeroDawnLevelGenerator : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnLevelGenerator();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void GenerateTrainingMap();

	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void GenerateMPMap_Nuketown();

	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void GenerateDominationMap();

	/** Generate a Search & Destroy map with bomb sites and team spawn points */
	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void GenerateSDMap();

	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void SpawnBuilding(FVector Location, FVector Extent, FColor Color);

	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void SpawnCover(FVector Location, float Height, float Width, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "LevelGen")
	void SpawnBarrier(FVector Location, float Height);

protected:
	UStaticMesh* BoxMesh;
	UStaticMesh* CylinderMesh;
	UMaterialInstanceDynamic* CreateMaterial(FColor Color);

	TArray<AActor*> SpawnedActors;
};
