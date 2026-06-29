#include "ZeroDawnLevelGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AZeroDawnLevelGenerator::AZeroDawnLevelGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (BoxFinder.Succeeded()) BoxMesh = BoxFinder.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylFinder.Succeeded()) CylinderMesh = CylFinder.Object;
}

void AZeroDawnLevelGenerator::BeginPlay()
{
	Super::BeginPlay();
}

UMaterialInstanceDynamic* AZeroDawnLevelGenerator::CreateMaterial(FColor Color)
{
	UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(
		UMaterial::GetDefaultMaterial(MD_Surface), this);
	if (Mat) Mat->SetVectorParameterValue("Color", FLinearColor(Color));
	return Mat;
}

void AZeroDawnLevelGenerator::SpawnBuilding(FVector Location, FVector Extent, FColor Color)
{
	if (!BoxMesh) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Building = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
	if (!Building) return;

	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Building);
	Mesh->RegisterComponent();
	Mesh->AttachToComponent(Building->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetStaticMesh(BoxMesh);
	Mesh->SetWorldScale3D(Extent / 100.0f);
	Mesh->SetMaterial(0, CreateMaterial(Color));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Building->SetRootComponent(Mesh);

	SpawnedActors.Add(Building);
}

void AZeroDawnLevelGenerator::SpawnCover(FVector Location, float Height, float Width, FRotator Rotation)
{
	if (!BoxMesh) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Cover = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), Location, Rotation, Params);
	if (!Cover) return;

	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Cover);
	Mesh->RegisterComponent();
	Mesh->AttachToComponent(Cover->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetStaticMesh(BoxMesh);
	Mesh->SetWorldScale3D(FVector(Width / 100.0f, 0.1f, Height / 100.0f));
	Mesh->SetMaterial(0, CreateMaterial(FColor(80, 80, 80)));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Cover->SetRootComponent(Mesh);

	SpawnedActors.Add(Cover);
}

void AZeroDawnLevelGenerator::SpawnBarrier(FVector Location, float Height)
{
	SpawnCover(Location, Height, 0.5f, FRotator::ZeroRotator);
}

void AZeroDawnLevelGenerator::GenerateTrainingMap()
{
	// Ground plane
	SpawnBuilding(FVector(0, 0, -50), FVector(5000, 5000, 100), FColor(60, 60, 60));

	// Walls - arena boundary
	SpawnBuilding(FVector(2500, 0, 250), FVector(100, 5000, 500), FColor(100, 100, 100));
	SpawnBuilding(FVector(-2500, 0, 250), FVector(100, 5000, 500), FColor(100, 100, 100));
	SpawnBuilding(FVector(0, 2500, 250), FVector(5000, 100, 500), FColor(100, 100, 100));
	SpawnBuilding(FVector(0, -2500, 250), FVector(5000, 100, 500), FColor(100, 100, 100));

	// Central structure
	SpawnBuilding(FVector(0, 0, 150), FVector(400, 400, 300), FColor(120, 120, 120));

	// Cover placements
	SpawnCover(FVector(500, 300, 150), 150, 2.0f, FRotator::ZeroRotator);
	SpawnCover(FVector(-500, -300, 150), 150, 2.0f, FRotator::ZeroRotator);
	SpawnCover(FVector(300, -500, 150), 150, 2.0f, FRotator(0, 90, 0));
	SpawnCover(FVector(-300, 500, 150), 150, 2.0f, FRotator(0, 90, 0));

	// Spawn points for Alpha and Bravo
	SpawnBuilding(FVector(-2000, 1500, 0), FVector(100, 100, 10), FColor(0, 100, 255));
	SpawnBuilding(FVector(2000, -1500, 0), FVector(100, 100, 10), FColor(255, 0, 0));
}

void AZeroDawnLevelGenerator::GenerateMPMap_Nuketown()
{
	// Classic CoD Nuketown-style map
	SpawnBuilding(FVector(0, 0, -50), FVector(4000, 4000, 100), FColor(50, 50, 50));

	// Boundary walls
	SpawnBuilding(FVector(2000, 0, 300), FVector(100, 4000, 600), FColor(90, 90, 90));
	SpawnBuilding(FVector(-2000, 0, 300), FVector(100, 4000, 600), FColor(90, 90, 90));
	SpawnBuilding(FVector(0, 2000, 300), FVector(4000, 100, 600), FColor(90, 90, 90));
	SpawnBuilding(FVector(0, -2000, 300), FVector(4000, 100, 600), FColor(90, 90, 90));

	// Two houses
	SpawnBuilding(FVector(-600, 0, 200), FVector(700, 700, 400), FColor(180, 160, 120));
	SpawnBuilding(FVector(600, 0, 200), FVector(700, 700, 400), FColor(180, 160, 120));

	// Center area
	SpawnBuilding(FVector(0, 0, 100), FVector(300, 300, 200), FColor(200, 80, 80));

	// Cars/cover
	SpawnCover(FVector(-200, -800, 80), 80, 2.0f, FRotator::ZeroRotator);
	SpawnCover(FVector(200, 800, 80), 80, 2.0f, FRotator::ZeroRotator);
	SpawnCover(FVector(-800, 200, 80), 80, 2.0f, FRotator(0, 90, 0));
	SpawnCover(FVector(800, -200, 80), 80, 2.0f, FRotator(0, 90, 0));

	// More cover
	SpawnBarrier(FVector(200, -1200, 100), 100);
	SpawnBarrier(FVector(-200, 1200, 100), 100);
	SpawnBarrier(FVector(-1200, -200, 100), 100);
	SpawnBarrier(FVector(1200, 200, 100), 100);
}

void AZeroDawnLevelGenerator::GenerateDominationMap()
{
	GenerateMPMap_Nuketown();

	// Domination zone markers
	SpawnBuilding(FVector(-300, 0, 25), FVector(60, 60, 50), FColor(0, 200, 255));
	SpawnBuilding(FVector(300, 0, 25), FVector(60, 60, 50), FColor(255, 200, 0));
	SpawnBuilding(FVector(0, 500, 25), FVector(60, 60, 50), FColor(0, 255, 50));
}
