#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnCharacter.generated.h"

class AZeroDawnWeapon;
class UZeroDawnWeaponComponent;
class UZeroDawnWeaponInventory;
class UZeroDawnKillstreakManager;
class UZeroDawnProgressionManager;
class UZeroDawnPerkSystem;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class IZeroDawnInteractable;

UCLASS(config=Game)
class AZeroDawnCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZeroDawnCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FirstPersonCameraComponent;

	// Health
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentArmor = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxArmor = 50.0f;

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddArmor(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsADS = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSliding = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 550.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float ADSSpeedMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed = 175.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SlideSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SlideDuration = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SlideCooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintToFireTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float JumpVelocity = 600.0f;

	float LastSlideTime;
	float SlideStartTime;
	bool bCanSlide = true;

	// Weapon system
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UZeroDawnWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UZeroDawnWeaponInventory* WeaponInventory;

	// Systems
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	UZeroDawnKillstreakManager* KillstreakManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	UZeroDawnProgressionManager* ProgressionManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	UZeroDawnPerkSystem* PerkSystem;

	// Team
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	ETeamType TeamType = ETeamType::None;

	UFUNCTION(BlueprintCallable, Category = "Team")
	void SetTeam(ETeamType NewTeam) { TeamType = NewTeam; }

	// Kill tracking
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Kills = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Deaths = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Assists = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 CurrentKillstreak = 0;

	// Perk-related properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perks")
	bool bColdBlooded = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perks")
	bool bGhost = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perks")
	bool bOverkill = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perks")
	float ExplosiveDamageMultiplier = 1.0f;

	// Health regen (affected by QuickFix perk)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthRegenDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthRegenRate = 5.0f;

	float LastDamageTime = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddKill();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddDeath();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddAssist();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	// Respawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float RespawnDelay = 5.0f;

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void RequestRespawn();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestRespawn();

	FTimerHandle RespawnTimerHandle;

	// Killcam
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Killcam")
	AActor* KillerActor = nullptr;

	// Interaction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractionRange = 300.0f;

	// Effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* BloodImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DeathExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundCue* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundCue* HitSound;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayHitEffect(FVector HitLocation, FRotator HitRotation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDeathEffect();

protected:
	void SetupInputs();

	// Input handlers
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void StartCrouch();
	void StopCrouch();
	void StartJump();
	void StopJump();
	void StartSlide();
	void StartADS();
	void StopADS();
	void Reload();
	void Fire();
	void StopFire();
	void Interact();
	void Melee();
	void InspectWeapon();

	// Movement
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	// Sprint to fire penalty
	float LastFireTime = 0.0f;
	float LastSprintEndTime = 0.0f;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(bool bNewSprinting);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetADS(bool bNewADS);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCrouching(bool bNewCrouching);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSliding(bool bNewSliding);

	// Weapon fire (server authoritative)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMelee();

	// Death
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnDeath();

	bool bIsDead = false;

	// Input
	class UEnhancedInputComponent* EnhancedInputComponent;
	class UInputMappingContext* InputMapping;
	class UInputAction* MoveAction;
	class UInputAction* LookAction;
	class UInputAction* SprintAction;
	class UInputAction* CrouchAction;
	class UInputAction* JumpAction;
	class UInputAction* SlideAction;
	class UInputAction* ADSAction;
	class UInputAction* FireAction;
	class UInputAction* ReloadAction;
	class UInputAction* InteractAction;
	class UInputAction* MeleeAction;
	class UInputAction* InspectAction;

	void CacheInputActions();
};
